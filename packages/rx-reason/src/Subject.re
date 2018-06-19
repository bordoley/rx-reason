type t('a) = {
  observer: Observer.t('a),
  observable: Observable.t('a),
};

let disposed = {observer: Observer.disposed, observable: Observable.never};

let complete = (~exn=?, {observer}) => observer |> Observer.complete(~exn?);

let completeWithResult = (~exn=?, {observer}) =>
  observer |> Observer.completeWithResult(~exn?);

let dispose = ({observer}) => observer |> Observer.dispose;

let isDisposed = ({observer}) => observer |> Observer.isDisposed;

let next = (next, {observer}) => observer |> Observer.next(next);

let notify = (notif, {observer}) => observer |> Observer.notify(notif);

let raiseIfDisposed = ({observer}) => observer |> Observer.raiseIfDisposed;

let asDisposable = ({observer}) => observer |> Observer.asDisposable;

let asObservable = ({observable}: t('a)) : Observable.t('a) => observable;

let publishTo = (~onNext, ~onComplete, subject) =>
  subject |> asObservable |> Observable.publishTo(~onNext, ~onComplete);

let publish =
    (~onNext=Functions.alwaysUnit, ~onComplete=Functions.alwaysUnit, subject) =>
  subject |> publishTo(~onNext, ~onComplete);

let subscribeWith = (~onNext, ~onComplete, subject) =>
  subject |> asObservable |> Observable.subscribeWith(~onNext, ~onComplete);

let subscribe =
    (~onNext=Functions.alwaysUnit, ~onComplete=Functions.alwaysUnit, subject) =>
  subject |> subscribeWith(~onNext, ~onComplete);

let createWithCallbacks =
    (~onNext, ~onComplete, ~onDispose, ~onSubscribe)
    : t('a) => {
  let subscribers = ref(CopyOnWriteArray.empty());
  let onComplete = exn => {
    let currentSubscribers = subscribers^;
    onComplete(exn);
    currentSubscribers
    |> CopyOnWriteArray.forEach(observer =>
         observer |> Observer.complete(~exn?)
       );
  };

  let onNext = next => {
    onNext(next);
    let currentSubscribers = subscribers^;
    currentSubscribers
    |> CopyOnWriteArray.forEach(observer => observer |> Observer.next(next));
  };
  let observer =
    Observer.create(~onComplete, ~onNext)
    |> Observer.addTeardown(onDispose)
    |> Observer.addTeardown(() => subscribers := CopyOnWriteArray.empty());

  let observable =
    Observable.create(subscriber => {
      observer |> Observer.raiseIfDisposed;
      subscribers := subscribers^ |> CopyOnWriteArray.addLast(subscriber);

      onSubscribe(subscriber);

      let teardown = () => {
        let currentSubscribers = subscribers^;
        subscribers :=
          currentSubscribers
          |> CopyOnWriteArray.findAndRemove(
               Functions.referenceEquality,
               subscriber,
             );
      };

      subscriber |> Observer.addTeardown(teardown) |> ignore;
    });
  {observer, observable};
};

let create = () =>
  createWithCallbacks(
    ~onNext=Functions.alwaysUnit,
    ~onComplete=Functions.alwaysUnit,
    ~onDispose=Functions.alwaysUnit,
    ~onSubscribe=Functions.alwaysUnit,
  );

let createWithReplayBuffer = (maxBufferCount: int) : t('a) => {
  /* This is fine for small buffers, eg. < 32
   * Ideally we'd use something like an Immutable.re vector
   * for large replay buffers.
   *
   * We use a cow array in order to snapshot the events we replay
   * in case their is feedback in the event system.
   */
  let buffer = ref(CopyOnWriteArray.empty());
  let completed = ref(false);
  let completedValue = ref(None);

  createWithCallbacks(
    ~onNext=
      next => {
        let currentBuffer = buffer^;
        let nextBuffer =
          if (CopyOnWriteArray.count(currentBuffer) === maxBufferCount) {
            currentBuffer |> CopyOnWriteArray.removeAt(0);
          } else {
            currentBuffer;
          };
        buffer := nextBuffer |> CopyOnWriteArray.addLast(next);
      },
    ~onComplete=
      exn => {
        completed := true;
        completedValue := exn;
      },
    ~onDispose=() => buffer := CopyOnWriteArray.empty(),
    ~onSubscribe=
      observer => {
        let currentBuffer = buffer^;
        CopyOnWriteArray.forEach(
          Observer.forwardOnNext(observer),
          currentBuffer,
        );
        if (completed^) {
          (); /*observer |> Observer.complete(~exn=(completedValue^)?);*/
        };
      },
  );
};
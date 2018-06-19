type t('a) = {
  subscriber: Subscriber.t('a),
  observable: Observable.t('a),
};

let disposed = {subscriber: Subscriber.disposed, observable: Observable.never};

let complete = (~exn=?, {subscriber}) => subscriber |> Subscriber.complete(~exn?);

let completeWithResult = (~exn=?, {subscriber}) =>
  subscriber |> Subscriber.completeWithResult(~exn?);

let dispose = ({subscriber}) => subscriber |> Subscriber.dispose;

let isDisposed = ({subscriber}) => subscriber |> Subscriber.isDisposed;

let isStopped = ({subscriber}) => subscriber |> Subscriber.isStopped;

let next = (next, {subscriber}) => subscriber |> Subscriber.next(next);

let notify = (notif, {subscriber}) => subscriber |> Subscriber.notify(notif);

let raiseIfDisposed = ({subscriber}) => subscriber |> Subscriber.raiseIfDisposed;

let asDisposable = ({subscriber}) => subscriber |> Subscriber.asDisposable;

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
    |> CopyOnWriteArray.forEach(subscriber =>
         subscriber |> Subscriber.complete(~exn?)
       );
  };

  let onNext = next => {
    onNext(next);
    let currentSubscribers = subscribers^;
    currentSubscribers
    |> CopyOnWriteArray.forEach(subscriber => subscriber |> Subscriber.next(next));
  };
  let subscriber =
    Subscriber.create(~onComplete, ~onNext)
    |> Subscriber.addTeardown(onDispose)
    |> Subscriber.addTeardown(() => subscribers := CopyOnWriteArray.empty());

  let observable =
    Observable.create(subscriber => {
      subscriber |> Subscriber.raiseIfDisposed;
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

      subscriber |> Subscriber.addTeardown(teardown) |> ignore;
    });
  {subscriber, observable};
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
      subscriber => {
        let currentBuffer = buffer^;
        CopyOnWriteArray.forEach(
          Subscriber.forwardOnNext(subscriber),
          currentBuffer,
        );
        if (completed^) {
          (); /*subscriber |> Subscriber.complete(~exn=(completedValue^)?);*/
        };
      },
  );
};
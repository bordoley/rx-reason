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

let isStopped = ({observer}) => observer |> Observer.isStopped;

let next = (next, {observer}) => observer |> Observer.next(next);

let notify = (notif, {observer}) => observer |> Observer.notify(notif);

let raiseIfDisposed = ({observer}) => observer |> Observer.raiseIfDisposed;

let asDisposable = ({observer}) => observer |> Observer.asDisposable;

let asObserver = ({observer}: t('a)) : Observer.t('a) => observer;

let asObservable = ({observable}: t('a)) : Observable.t('a) => observable;

let publishObserver = (observer, subject) =>
  subject |> asObservable |> Observable.publishObserver(observer);

let publishWithCallbacks = (~onNext, ~onComplete, subject) =>
  subject
  |> asObservable
  |> Observable.publishWithCallbacks(~onNext, ~onComplete);

let publish =
    (~onNext=Functions.alwaysUnit, ~onComplete=Functions.alwaysUnit, subject) =>
  subject |> publishWithCallbacks(~onNext, ~onComplete);

let subscribeObserver = (observer, subject) =>
  subject |> asObservable |> Observable.subscribeObserver(observer);

let subscribeWithCallbacks = (~onNext, ~onComplete, subject) =>
  subject
  |> asObservable
  |> Observable.subscribeWithCallbacks(~onNext, ~onComplete);

let subscribe =
    (~onNext=Functions.alwaysUnit, ~onComplete=Functions.alwaysUnit, subject) =>
  subject |> subscribeWithCallbacks(~onNext, ~onComplete);

let createWithCallbacks =
    (~onNext, ~onComplete, ~onDispose, ~onSubscribe)
    : t('a) => {
  let subscribers = ref(CopyOnWriteArray.empty());
  let observer =
    Observer.create(
      ~onComplete=
        exn => {
          let currentSubscribers = subscribers^;
          onComplete(exn);
          currentSubscribers
          |> CopyOnWriteArray.forEach(((_, onComplete)) => onComplete(exn));
        },
      ~onNext=
        next => {
          onNext(next);
          let currentSubscribers = subscribers^;
          currentSubscribers
          |> CopyOnWriteArray.forEach(((onNext, _)) => onNext(next));
        },
      ~onDispose=
        () => {
          onDispose();
          subscribers := CopyOnWriteArray.empty();
        },
    );

  let observable =
    Observable.create((~onNext, ~onComplete) => {
      observer |> Observer.raiseIfDisposed;

      let subscriber = (onNext, onComplete);
      subscribers := subscribers^ |> CopyOnWriteArray.addLast(subscriber);

      onSubscribe(~onNext, ~onComplete);

      Disposable.create(() => {
        let currentSubscribers = subscribers^;
        subscribers :=
          currentSubscribers
          |> CopyOnWriteArray.findAndRemove(
               Functions.referenceEquality,
               subscriber,
             );
      });
    });
  {observer, observable};
};

let create = () =>
  createWithCallbacks(
    ~onNext=Functions.alwaysUnit,
    ~onComplete=Functions.alwaysUnit,
    ~onDispose=Functions.alwaysUnit,
    ~onSubscribe=(~onNext as _, ~onComplete as _) =>
    ()
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
      (~onNext, ~onComplete) => {
        let currentBuffer = buffer^;
        CopyOnWriteArray.forEach(onNext, currentBuffer);
        if (completed^) {
          onComplete(completedValue^);
        };
      },
  );
};
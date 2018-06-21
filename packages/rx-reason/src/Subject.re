type t('a) = {
  subscriber: Subscriber.t('a),
  observable: Observable.t('a),
  disposable: Disposable.t,
};

let disposed = {
  subscriber: Subscriber.disposed,
  observable: Observable.never,
  disposable: Disposable.disposed,
};

let asDisposable = ({disposable}) => disposable;

let asObservable = ({observable}) => observable;

let complete = (~exn=?, {subscriber}) =>
  subscriber |> Subscriber.complete(~exn?);

let completeWithResult = (~exn=?, {subscriber}) =>
  subscriber |> Subscriber.completeWithResult(~exn?);

let dispose = self => self |> asDisposable |> Disposable.dispose;

let isDisposed = self => self |> asDisposable |> Disposable.isDisposed;

let next = (next, {subscriber}) => subscriber |> Subscriber.next(next);

let notify = (notif, {subscriber}) =>
  subscriber |> Subscriber.notify(notif);

let raiseIfDisposed = self =>
  self |> asDisposable |> Disposable.raiseIfDisposed;

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
    |> CopyOnWriteArray.forEach(subscriber =>
         subscriber |> Subscriber.next(next)
       );
  };

  let onComplete = exn => {
    onComplete(exn);
    let currentSubscribers = subscribers^;
    currentSubscribers
    |> CopyOnWriteArray.forEach(subscriber =>
         subscriber |> Subscriber.complete(~exn?)
       );
  };

  let subscriber = Subscriber.createAutoDisposing(~onNext, ~onComplete);

  let disposable =
    Disposable.create(() => {
      subscriber |> Subscriber.dispose;
      onDispose();
      subscribers := CopyOnWriteArray.empty();
    });

  let observable =
    Observable.create(subscriber => {
      disposable |> Disposable.raiseIfDisposed;
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

  {subscriber, observable, disposable};
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
          next => subscriber |> Subscriber.next(next),
          currentBuffer,
        );
        if (completed^) {
          let exn = completedValue^;
          subscriber |> Subscriber.complete(~exn?);
        };
      },
  );
};
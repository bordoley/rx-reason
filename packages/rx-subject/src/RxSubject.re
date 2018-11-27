type t('a) = {
  subscriber: RxSubscriber.t('a),
  observable: RxObservable.t('a),
  disposable: RxDisposable.t,
};

let disposed = {
  subscriber: RxSubscriber.disposed,
  observable: RxObservable.never,
  disposable: RxDisposable.disposed,
};

let asDisposable = ({disposable}) => disposable;

let asObservable = ({observable}) => observable;

let complete = (~exn=?, {subscriber}) =>
  subscriber |> RxSubscriber.complete(~exn?);

let completeWithResult = (~exn=?, {subscriber}) =>
  subscriber |> RxSubscriber.completeWithResult(~exn?);

let dispose = self => self |> asDisposable |> RxDisposable.dispose;

let isDisposed = self => self |> asDisposable |> RxDisposable.isDisposed;

let next = (next, {subscriber}) => subscriber |> RxSubscriber.next(next);

let forwardOnComplete = (subject, exn) => subject |> complete(~exn?);

let forwardOnNext = (subject, v) => subject |> next(v);

let notify = (notif, {subscriber}) =>
  subscriber |> RxSubscriber.notify(notif);

let raiseIfDisposed = self =>
  self |> asDisposable |> RxDisposable.raiseIfDisposed;

let subscribeWith = (subscriber, subject) =>
  subject |> asObservable |> RxObservable.subscribeWith(subscriber);

let subscribe = subject => subject |> asObservable |> RxObservable.subscribe;

let create = {
  let teardown = (subscriber, subscribers) => {
    let currentSubscribers = subscribers^;
    subscribers :=
      currentSubscribers
      |> RxCopyOnWriteArray.findAndRemoveReference(subscriber);
  };

  let subscriberOnNext = (subscribers, next) => {
    let currentSubscribers = subscribers^;
    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.next(next)
       );
  };

  let subscriberOnComplete = (subscribers, exn) => {
    let currentSubscribers = subscribers^;
    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.complete(~exn?)
       );
  };

  let disposableTeardown = (subscriber, subscribers) => {
    subscriber |> RxSubscriber.dispose;
    subscribers := RxCopyOnWriteArray.empty();
  };

  let observableSource = (subscribers, disposable, subscriber) => {
    disposable |> RxDisposable.raiseIfDisposed;
    subscribers := subscribers^ |> RxCopyOnWriteArray.addLast(subscriber);

    subscriber
    |> RxSubscriber.addTeardown2(teardown, subscriber, subscribers)
    |> ignore;
  };

  () => {
    let subscribers = ref(RxCopyOnWriteArray.empty());

    let subscriber =
      RxSubscriber.create1(
        ~onNext=subscriberOnNext,
        ~onComplete=subscriberOnComplete,
        subscribers,
      );

    let disposable =
      RxDisposable.create2(disposableTeardown, subscriber, subscribers);

    let observable =
      RxObservable.create2(observableSource, subscribers, disposable);

    {subscriber, observable, disposable};
  };
};

let createWithReplayBuffer = {
  let teardown = (subscriber, subscribers) => {
    let currentSubscribers = subscribers^;
    subscribers :=
      currentSubscribers
      |> RxCopyOnWriteArray.findAndRemoveReference(subscriber);
  };

  let subscriberOnNext = (maxBufferCount, buffer, _, _, subscribers, next) => {
    let currentBuffer = buffer^;
    buffer :=
      currentBuffer
      |> RxCopyOnWriteArray.addLastWithMaxCount(maxBufferCount, next);

    let currentSubscribers = subscribers^;
    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.next(next)
       );
  };

  let subscriberOnComplete =
      (_, _, completed, completedValue, subscribers, exn) => {
    completed := true;
    completedValue := exn;

    let currentSubscribers = subscribers^;
    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.complete(~exn?)
       );
  };

  let disposableTeardown = (buffer, subscriber, subscribers) => {
    buffer := RxCopyOnWriteArray.empty();
    subscriber |> RxSubscriber.dispose;
    subscribers := RxCopyOnWriteArray.empty();
  };

  (maxBufferCount: int) => {
    /* This is fine for small buffers, eg. < 32
     * Ideally we'd use something like an Immutable.re vector
     * for large replay buffers.
     *
     * We use a cow array in order to snapshot the events we replay
     * in case their is feedback in the event system.
     */
    let buffer = ref(RxCopyOnWriteArray.empty());
    let completed = ref(false);
    let completedValue = ref(None);

    let subscribers = ref(RxCopyOnWriteArray.empty());

    let subscriber =
      RxSubscriber.create5(
        ~onNext=subscriberOnNext,
        ~onComplete=subscriberOnComplete,
        maxBufferCount,
        buffer,
        completed,
        completedValue,
        subscribers,
      );

    let disposable =
      RxDisposable.create3(
        disposableTeardown,
        buffer,
        subscriber,
        subscribers,
      );

    let observable =
      RxObservable.create(subscriber => {
        disposable |> RxDisposable.raiseIfDisposed;
        subscribers := subscribers^ |> RxCopyOnWriteArray.addLast(subscriber);

        let currentBuffer = buffer^;
        RxCopyOnWriteArray.forEach(
          next => subscriber |> RxSubscriber.next(next),
          currentBuffer,
        );
        if (completed^) {
          let exn = completedValue^;
          subscriber |> RxSubscriber.complete(~exn?);
        };

        subscriber
        |> RxSubscriber.addTeardown2(teardown, subscriber, subscribers)
        |> ignore;
      });

    {subscriber, observable, disposable};
  };
};
type subscribers('a) = ref(RxCopyOnWriteArray.t(RxSubscriber.t('a)));

type onNext0('a) = ('a, subscribers('a)) => unit;
type onNext1('a, 'ctx0) = ('ctx0, 'a, subscribers('a)) => unit;
type onNext2('a, 'ctx0, 'ctx1) = ('ctx0, 'ctx1, 'a, subscribers('a)) => unit;

type onComplete0('a) = (option(exn), subscribers('a)) => unit;
type onComplete1('a, 'ctx0) = ('ctx0, option(exn), subscribers('a)) => unit;
type onComplete2('a, 'ctx0, 'ctx1) =
  ('ctx0, 'ctx1, option(exn), subscribers('a)) => unit;

type t('a) =
  | Disposed
  | S0(
        subscribers('a),
        RxObservable.t('a),
        RxDisposable.t,
        RxAtomic.t(bool),
        onNext0('a),
        onComplete0('a),
      ): t('a)
  | S1(
        subscribers('a),
        RxObservable.t('a),
        RxDisposable.t,
        RxAtomic.t(bool),
        onNext1('a, 'ctx0),
        onComplete1('a, 'ctx0),
        'ctx0,
      ): t('a)
  | S2(
        subscribers('a),
        RxObservable.t('a),
        RxDisposable.t,
        RxAtomic.t(bool),
        onNext2('a, 'ctx0, 'ctx1),
        onComplete2('a, 'ctx0, 'ctx1),
        'ctx0,
        'ctx1,
      ): t('a);

let disposed = Disposed;

let asDisposable =
  fun
  | Disposed => RxDisposable.disposed
  | S0(_, _, disposable, _, _, _)
  | S1(_, _, disposable, _, _, _, _)
  | S2(_, _, disposable, _, _, _, _, _) => disposable;

let asObservable =
  fun
  | Disposed => RxObservable.never
  | S0(_, obs, _, _, _, _)
  | S1(_, obs, _, _, _, _, _)
  | S2(_, obs, _, _, _, _, _, _) => obs;

let complete = (~exn=?) =>
  fun
  | Disposed => ()
  | S0(subs, _, _, isStopped, _, onComplete) =>
    if (! RxAtomic.exchange(isStopped, true)) {
      onComplete(exn, subs);
    }
  | S1(subs, _, _, isStopped, _, onComplete, ctx0) =>
    if (! RxAtomic.exchange(isStopped, true)) {
      onComplete(ctx0, exn, subs);
    }
  | S2(subs, _, _, isStopped, _, onComplete, ctx0, ctx1) =>
    if (! RxAtomic.exchange(isStopped, true)) {
      onComplete(ctx0, ctx1, exn, subs);
    };

let dispose = self => self |> asDisposable |> RxDisposable.dispose;

let isDisposed = self => self |> asDisposable |> RxDisposable.isDisposed;

let next = next =>
  fun
  | Disposed => ()
  | S0(subs, _, _, isStopped, onNext, _) =>
    if (! RxAtomic.get(isStopped)) {
      onNext(next, subs);
    }
  | S1(subs, _, _, isStopped, onNext, _, ctx0) =>
    if (! RxAtomic.get(isStopped)) {
      onNext(ctx0, next, subs);
    }
  | S2(subs, _, _, isStopped, onNext, _, ctx0, ctx1) =>
    if (! RxAtomic.get(isStopped)) {
      onNext(ctx0, ctx1, next, subs);
    };

let forwardOnComplete = (subject, exn) => subject |> complete(~exn?);

let forwardOnNext = (subject, v) => subject |> next(v);

let notify = (notif, subject) =>
  switch (notif) {
  | RxNotification.Next(v) => subject |> next(v)
  | RxNotification.Complete(exn) => subject |> complete(~exn?)
  };

let raiseIfDisposed = self =>
  self |> asDisposable |> RxDisposable.raiseIfDisposed;

let create = {
  let subscriberTeardown = (subscriber, subscribers) => {
    let currentSubscribers = subscribers^;
    subscribers :=
      currentSubscribers
      |> RxCopyOnWriteArray.findAndRemoveReference(subscriber);
  };

  let observableSource = (subscribers, disposable, subscriber) => {
    disposable |> RxDisposable.raiseIfDisposed;
    subscribers := subscribers^ |> RxCopyOnWriteArray.addLast(subscriber);

    subscriber
    |> RxSubscriber.addTeardown2(subscriberTeardown, subscriber, subscribers)
    |> ignore;
  };

  let subscribersTeardown = subscribers =>
    subscribers := RxCopyOnWriteArray.empty();

  let onComplete = (exn, subscribers) => {
    let currentSubscribers = subscribers^;

    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.complete(~exn?)
       );
  };

  let onNext = (next, subscribers) => {
    let currentSubscribers = subscribers^;
    
    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.next(next)
       );
  };

  () => {
    let subscribers = ref(RxCopyOnWriteArray.empty());
    let disposable = RxDisposable.create1(subscribersTeardown, subscribers);
    let observable =
      RxObservable.create2(observableSource, subscribers, disposable);

    S0(
      subscribers,
      observable,
      disposable,
      RxAtomic.make(false),
      onNext,
      onComplete,
    );
  };
};

let createWithReplayBuffer = {
  let subscriberTeardown = (subscriber, subscribers) => {
    let currentSubscribers = subscribers^;
    subscribers :=
      currentSubscribers
      |> RxCopyOnWriteArray.findAndRemoveReference(subscriber);
  };

  let observableSource = (buffer, disposable, subscribers, subscriber) => {
    disposable |> RxDisposable.raiseIfDisposed;
    subscribers := subscribers^ |> RxCopyOnWriteArray.addLast(subscriber);

    let currentBuffer = buffer^;
    RxCopyOnWriteArray.forEach(
      next => subscriber |> RxSubscriber.notify(next),
      currentBuffer,
    );

    subscriber
    |> RxSubscriber.addTeardown2(subscriberTeardown, subscriber, subscribers)
    |> ignore;
  };

  let onNext = (maxBufferCount, buffer, next, subscribers) => {
    let currentBuffer = buffer^;
    let currentSubscribers = subscribers^;

    buffer :=
      currentBuffer
      |> RxCopyOnWriteArray.addLastWithMaxCount(
           maxBufferCount,
           RxNotification.next(next),
         );

    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.next(next)
       );
  };

  let onComplete = (maxBufferCount, buffer, exn, subscribers) => {
    let currentBuffer = buffer^;
    let currentSubscribers = subscribers^;

    buffer :=
      currentBuffer
      |> RxCopyOnWriteArray.addLastWithMaxCount(
           maxBufferCount,
           RxNotification.complete(exn),
         );

    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.complete(~exn?)
       );
  };

  let subscribersTeardown = (buffer, subscribers) => {
    buffer := RxCopyOnWriteArray.empty();
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
    let subscribers = ref(RxCopyOnWriteArray.empty());

    let disposable =
      RxDisposable.create2(subscribersTeardown, buffer, subscribers);

    let observable =
      RxObservable.create3(observableSource, buffer, disposable, subscribers);

    S2(
      subscribers,
      observable,
      disposable,
      RxAtomic.make(false),
      onNext,
      onComplete,
      maxBufferCount,
      buffer,
    );
  };
};
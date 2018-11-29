type subscribers('a) = ref(RxCopyOnWriteArray.t(RxSubscriber.t('a)));

type onNext('a) = ('a, subscribers('a)) => unit;
type onNext2('a, 'ctx0, 'ctx1) = ('ctx0, 'ctx1, 'a, subscribers('a)) => unit;

type onComplete('a) = (option(exn), subscribers('a)) => unit;
type onComplete2('a, 'ctx0, 'ctx1) =
  ('ctx0, 'ctx1, option(exn), subscribers('a)) => unit;

type t('a) =
  | Disposed
  | S0(
        subscribers('a),
        RxObservable.t('a),
        RxDisposable.t,
        RxAtomic.t(bool),
        onNext('a),
        onComplete('a),
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
  | S2(_, _, disposable, _, _, _, _, _) => disposable;

let asObservable =
  fun
  | Disposed => RxObservable.never
  | S0(_, obs, _, _, _, _)
  | S2(_, obs, _, _, _, _, _, _) => obs;

let complete = (~exn=?) =>
  fun
  | Disposed => ()
  | S0(subs, _, _, isStopped, _, onComplete) =>
    if (! RxAtomic.exchange(isStopped, true)) {
      onComplete(exn, subs);
      subs := RxCopyOnWriteArray.empty();
    }
  | S2(subs, _, _, isStopped, _, onComplete, ctx0, ctx1) =>
    if (! RxAtomic.exchange(isStopped, true)) {
      onComplete(ctx0, ctx1, exn, subs);
      subs := RxCopyOnWriteArray.empty();
    };

let dispose =
  fun
  | Disposed => ()
  | S0(subs, _, disposable, isStopped, _, _)
  | S2(subs, _, disposable, isStopped, _, _, _, _) => {
      if (! RxAtomic.exchange(isStopped, true)) {
        subs := RxCopyOnWriteArray.empty();
      };
      disposable |> RxDisposable.dispose;
    };

let isDisposed = self => self |> asDisposable |> RxDisposable.isDisposed;

let next = next =>
  fun
  | Disposed => ()
  | S0(subs, _, _, isStopped, onNext, _) =>
    if (! RxAtomic.get(isStopped)) {
      onNext(next, subs);
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

module Subject = {
  let subscriberTeardown = (subscriber, subscribers) => {
    let currentSubscribers = subscribers^;
    subscribers :=
      currentSubscribers
      |> RxCopyOnWriteArray.findAndRemoveReference(subscriber);
  };

  let observableSource = (subscribers, disposable, isStopped, subscriber) => {
    disposable |> RxDisposable.raiseIfDisposed;

    if (RxAtomic.get(isStopped)) {
      subscriber |> RxSubscriber.dispose;
    } else {
      subscribers := subscribers^ |> RxCopyOnWriteArray.addLast(subscriber);

      subscriber
      |> RxSubscriber.addTeardown2(subscriberTeardown, subscriber, subscribers)
      |> ignore;
      }
  };

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

  let create = () => {
    let subscribers = ref(RxCopyOnWriteArray.empty());
    let disposable = RxDisposable.empty();
    let isStopped = RxAtomic.make(false);

    let observable =
      RxObservable.create3(observableSource, subscribers, disposable, isStopped);

    S0(
      subscribers,
      observable,
      disposable,
      isStopped,
      onNext,
      onComplete,
    );
  };
};

module ReplayBufferSubject = {
  let observableSource = (buffer, subscribers, disposable, isStopped, subscriber) => {
    disposable |> RxDisposable.raiseIfDisposed;

    let currentBuffer = buffer^;
    RxCopyOnWriteArray.forEach(
      next => subscriber |> RxSubscriber.notify(next),
      currentBuffer,
    );

    Subject.observableSource(subscribers, disposable, isStopped, subscriber);
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

  let bufferTeardown = (buffer) => {
    buffer := RxCopyOnWriteArray.empty();
  };

  let create = (maxBufferCount: int) => {
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
      RxDisposable.create1(bufferTeardown, buffer);

    let isStopped = RxAtomic.make(false);

    let observable =
      RxObservable.create4(observableSource, buffer, subscribers, disposable, isStopped);

    S2(
      subscribers,
      observable,
      disposable,
      isStopped,
      onNext,
      onComplete,
      maxBufferCount,
      buffer,
    );
  };
};

let create = Subject.create;
let createWithReplayBuffer = ReplayBufferSubject.create;
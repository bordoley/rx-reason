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
  | S0(_, observable, _, _, _, _)
  | S2(_, observable, _, _, _, _, _, _) => observable;

let dispose = subject => subject |> asDisposable |> RxDisposable.dispose;

let isDisposed = subject => subject |> asDisposable |> RxDisposable.isDisposed;

let raiseIfDisposed = subject =>
  subject |> asDisposable |> RxDisposable.raiseIfDisposed;

let shouldComplete =
  fun
  | Disposed => false
  | S0(_, _, _, isStopped, _, _)
  | S2(_, _, _, isStopped, _, _, _, _) =>
    ! RxAtomic.exchange(isStopped, true);

let completeWithResult = (~exn=?, subscriber) => {
  let shouldComplete = shouldComplete(subscriber);

  if (shouldComplete) {
    switch (subscriber) {
    | Disposed => ()
    | S0(subscribers, _, _, _, _, onComplete) => onComplete(exn, subscribers)
    | S2(subscribers, _, _, _, _, onComplete, ctx0, ctx1) =>
      onComplete(ctx0, ctx1, exn, subscribers)
    };
  };
  shouldComplete;
};

let complete = (~exn=?, subscriber) =>
  subscriber |> completeWithResult(~exn?) |> ignore;

let isStopped =
  fun
  | Disposed => true
  | S0(_, _, _, isStopped, _, _)
  | S2(_, _, _, isStopped, _, _, _, _) => RxAtomic.get(isStopped);

let next = (next, subject) =>
  if (! isStopped(subject)) {
    switch (subject) {
    | Disposed => ()
    | S0(subscribers, _, _, _, onNext, _) => onNext(next, subscribers)
    | S2(subscribers, _, _, _, onNext, _, ctx0, ctx1) =>
      onNext(ctx0, ctx1, next, subscribers)
    };
  };

let notify = (notif, subject) =>
  switch (notif) {
  | RxNotification.Next(v) => subject |> next(v)
  | RxNotification.Complete(exn) => subject |> complete(~exn?)
  };

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
      |> RxSubscriber.addTeardown2(
           subscriberTeardown,
           subscriber,
           subscribers,
         )
      |> ignore;
    };
  };

  let onComplete = (exn, subscribers) => {
    let currentSubscribers = subscribers^;

    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.complete(~exn?)
       );

    subscribers := RxCopyOnWriteArray.empty();
  };

  let onNext = (next, subscribers) => {
    let currentSubscribers = subscribers^;

    currentSubscribers
    |> RxCopyOnWriteArray.forEach(subscriber =>
         subscriber |> RxSubscriber.next(next)
       );
  };

  let disposableTeardown = (isStopped, subscribers) => {
    RxAtomic.set(isStopped, true);
    subscribers := RxCopyOnWriteArray.empty();
  };

  let create = () => {
    let subscribers = ref(RxCopyOnWriteArray.empty());
    let disposable = RxDisposable.empty();
    let isStopped = RxAtomic.make(false);

    let observable =
      RxObservable.create3(
        observableSource,
        subscribers,
        disposable,
        isStopped,
      );

    S0(subscribers, observable, disposable, isStopped, onNext, onComplete);
  };
};

module ReplayBufferSubject = {
  let observableSource =
      (buffer, subscribers, disposable, isStopped, subscriber) => {
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

    buffer :=
      currentBuffer
      |> RxCopyOnWriteArray.addLastWithMaxCount(
           maxBufferCount,
           RxNotification.next(next),
         );

    Subject.onNext(next, subscribers);
  };

  let onComplete = (maxBufferCount, buffer, exn, subscribers) => {
    let currentBuffer = buffer^;

    buffer :=
      currentBuffer
      |> RxCopyOnWriteArray.addLastWithMaxCount(
           maxBufferCount,
           RxNotification.complete(exn),
         );

    Subject.onComplete(exn, subscribers);
  };

  let disposableTeardown = (buffer, isStopped, subscribers) => {
    Subject.disposableTeardown(isStopped, subscribers);
    buffer := RxCopyOnWriteArray.empty();
  };

  let create = (maxBufferCount: int) => {
    /* This is fine for small buffers, eg. < 32
     * Ideally we'd use something like an Immutable.re vector
     * for large replay buffers.
     *
     * We use a cow array in order to snapshot the events we replay
     * in case there is feedback in the event system.
     */
    let buffer = ref(RxCopyOnWriteArray.empty());

    let subscribers = ref(RxCopyOnWriteArray.empty());
    let isStopped = RxAtomic.make(false);
    let disposable =
      RxDisposable.create3(
        disposableTeardown,
        buffer,
        isStopped,
        subscribers,
      );

    let observable =
      RxObservable.create4(
        observableSource,
        buffer,
        subscribers,
        disposable,
        isStopped,
      );

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
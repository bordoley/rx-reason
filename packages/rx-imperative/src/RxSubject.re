type subscribers('a) = ref(RxCopyOnWriteArray.t(RxSubscriber.t('a)));

type onNext('a) = 'a => unit;
type onNext2('a, 'ctx0, 'ctx1) = ('ctx0, 'ctx1, 'a) => unit;

type onComplete('a) = option(exn) => unit;
type onComplete2('a, 'ctx0, 'ctx1) = ('ctx0, 'ctx1, option(exn)) => unit;

type onSubscribe('a) = RxSubscriber.t('a) => unit;
type onSubscribe2('a, 'ctx0, 'ctx1) =
  ('ctx0, 'ctx1, RxSubscriber.t('a)) => unit;

type onDispose = unit => unit;
type onDispose2('ctx0, 'ctx1) = ('ctx0, 'ctx1) => unit;

type t('a) =
  | Disposed
  | S0(
        subscribers('a),
        RxObservable.t('a),
        RxDisposable.t,
        RxAtomic.t(bool),
        onNext('a),
        onComplete('a),
        onSubscribe('a),
        onDispose,
      ): t('a)
  | S2(
        subscribers('a),
        RxObservable.t('a),
        RxDisposable.t,
        RxAtomic.t(bool),
        onNext2('a, 'ctx0, 'ctx1),
        onComplete2('a, 'ctx0, 'ctx1),
        onSubscribe2('a, 'ctx0, 'ctx1),
        onDispose2('ctx0, 'ctx1),
        'ctx0,
        'ctx1,
      ): t('a);

let disposed = Disposed;

let asDisposable =
  fun
  | Disposed => RxDisposable.disposed
  | S0(_, _, disposable, _, _, _, _, _)
  | S2(_, _, disposable, _, _, _, _, _, _, _) => disposable;

let asObservable =
  fun
  | Disposed => RxObservable.never
  | S0(_, observable, _, _, _, _, _, _)
  | S2(_, observable, _, _, _, _, _, _, _, _) => observable;

let dispose = subject => subject |> asDisposable |> RxDisposable.dispose;

let isDisposed = subject => subject |> asDisposable |> RxDisposable.isDisposed;

let raiseIfDisposed = subject =>
  subject |> asDisposable |> RxDisposable.raiseIfDisposed;

let shouldComplete =
  fun
  | Disposed => false
  | S0(_, _, _, isStopped, _, _, _, _)
  | S2(_, _, _, isStopped, _, _, _, _, _, _) =>
    ! RxAtomic.exchange(isStopped, true);

let notifyComplete = (exn, subscribers) => {
  let currentSubscribers = subscribers^;

  currentSubscribers
  |> RxCopyOnWriteArray.forEach(subscriber =>
       subscriber |> RxSubscriber.complete(~exn?)
     );

  subscribers := RxCopyOnWriteArray.empty();
};

let completeWithResult = (~exn=?, subject) => {
  let shouldComplete = shouldComplete(subject);

  if (shouldComplete) {
    switch (subject) {
    | Disposed => ()
    | S0(subscribers, _, _, _, _, onComplete, _, _) =>
      onComplete(exn);
      notifyComplete(exn, subscribers);
    | S2(subscribers, _, _, _, _, onComplete, _, _, ctx0, ctx1) =>
      onComplete(ctx0, ctx1, exn);
      notifyComplete(exn, subscribers);
    };
  };
  shouldComplete;
};

let complete = (~exn=?, subject) =>
  subject |> completeWithResult(~exn?) |> ignore;

let isStopped =
  fun
  | Disposed => true
  | S0(_, _, _, isStopped, _, _, _, _)
  | S2(_, _, _, isStopped, _, _, _, _, _, _) => RxAtomic.get(isStopped);

let notifyNext = (next, subscribers) => {
  let currentSubscribers = subscribers^;
  currentSubscribers
  |> RxCopyOnWriteArray.forEach(subscriber =>
       subscriber |> RxSubscriber.next(next)
     );
};

let next = (next, subject) =>
  if (! isStopped(subject)) {
    switch (subject) {
    | Disposed => ()
    | S0(subscribers, _, _, _, onNext, _, _, _) =>
      onNext(next);
      notifyNext(next, subscribers);
    | S2(subscribers, _, _, _, onNext, _, _, _, ctx0, ctx1) =>
      onNext(ctx0, ctx1, next);
      notifyNext(next, subscribers);
    };
  };

let subscriberTeardown = (subscriber, subscribers) => {
  let currentSubscribers = subscribers^;
  subscribers :=
    currentSubscribers
    |> RxCopyOnWriteArray.findAndRemoveReference(subscriber);
};

let subscribeOrDisposeSubscriber = (subscribers, isStopped, subscriber) =>
  if (RxAtomic.get(isStopped)) {
    subscriber |> RxSubscriber.dispose;
  } else {
    subscribers := subscribers^ |> RxCopyOnWriteArray.addLast(subscriber);

    let disposable =
      RxDisposable.create2(subscriberTeardown, subscriber, subscribers);

    subscriber |> RxSubscriber.addDisposable(disposable) |> ignore;
  };

let observableSource = (self, subscriber) => {
  let self = self^;
  self |> raiseIfDisposed;

  switch (self) {
  | Disposed => ()
  | S0(subscribers, _, _, isStopped, _, _, onSubscribe, _) =>
    onSubscribe(subscriber);
    subscribeOrDisposeSubscriber(subscribers, isStopped, subscriber);

  | S2(subscribers, _, _, isStopped, _, _, onSubscribe, _, ctx0, ctx1) =>
    onSubscribe(ctx0, ctx1, subscriber);
    subscribeOrDisposeSubscriber(subscribers, isStopped, subscriber);
  };
};

let disposableTeardown = {
  let stopAndClearSubscribers = (subscribers, isStopped) => {
    RxAtomic.set(isStopped, true);
    subscribers := RxCopyOnWriteArray.empty();
  };

  self =>
    switch (self^) {
    | Disposed => ()
    | S0(subscribers, _, _, isStopped, _, _, _, onDispose) =>
      stopAndClearSubscribers(subscribers, isStopped);
      onDispose();
    | S2(subscribers, _, _, isStopped, _, _, _, onDispose, ctx0, ctx1) =>
      stopAndClearSubscribers(subscribers, isStopped);
      onDispose(ctx0, ctx1);
    };
};

let create = (~onNext, ~onComplete, ~onSubscribe, ~onDispose) => {
  let self = ref(Disposed);

  let subscribers = ref(RxCopyOnWriteArray.empty());
  let observable = RxObservable.create1(observableSource, self);
  let disposable = RxDisposable.create1(disposableTeardown, self);
  let isStopped = RxAtomic.make(false);

  self :=
    S0(
      subscribers,
      observable,
      disposable,
      isStopped,
      onNext,
      onComplete,
      onSubscribe,
      onDispose,
    );
  self^;
};

let create2 = (~onNext, ~onComplete, ~onSubscribe, ~onDispose, ctx0, ctx1) => {
  let self = ref(Disposed);

  let subscribers = ref(RxCopyOnWriteArray.empty());
  let observable = RxObservable.create1(observableSource, self);
  let disposable = RxDisposable.create1(disposableTeardown, self);
  let isStopped = RxAtomic.make(false);

  self :=
    S2(
      subscribers,
      observable,
      disposable,
      isStopped,
      onNext,
      onComplete,
      onSubscribe,
      onDispose,
      ctx0,
      ctx1,
    );
  self^;
};
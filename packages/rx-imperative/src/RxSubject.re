type subscribers('a) = ref(RxCopyOnWriteArray.t(RxSubscriber.t('a)));

type onNext1('a, 'ctx0) = ('ctx0, 'a) => unit;
type onComplete1('a, 'ctx0) = ('ctx0, option(exn)) => unit;
type onSubscribe1('a, 'ctx0) = ('ctx0, RxSubscriber.t('a)) => unit;
type onDispose1('ctx0) = 'ctx0 => unit;

type onNext2('a, 'ctx0, 'ctx1) = ('ctx0, 'ctx1, 'a) => unit;
type onComplete2('a, 'ctx0, 'ctx1) = ('ctx0, 'ctx1, option(exn)) => unit;
type onSubscribe2('a, 'ctx0, 'ctx1) =
  ('ctx0, 'ctx1, RxSubscriber.t('a)) => unit;
type onDispose2('ctx0, 'ctx1) = ('ctx0, 'ctx1) => unit;

type t('a) =
  | Disposed
  | Multicast(subscribers('a), RxObservable.t('a), RxDisposable.t)
  | S1(
      subscribers('a),
      RxObservable.t('a),
      RxDisposable.t,
      onNext1('a, 'ctx0),
      onComplete1('a, 'ctx0),
      onSubscribe1('a, 'ctx0),
      onDispose1('ctx0),
      'ctx0,
    )
    : t('a)
  | S2(
      subscribers('a),
      RxObservable.t('a),
      RxDisposable.t,
      onNext2('a, 'ctx0, 'ctx1),
      onComplete2('a, 'ctx0, 'ctx1),
      onSubscribe2('a, 'ctx0, 'ctx1),
      onDispose2('ctx0, 'ctx1),
      'ctx0,
      'ctx1,
    )
    : t('a);

let disposed = Disposed;

let asDisposable =
  fun
  | Disposed => RxDisposable.disposed
  | Multicast(_, _, disposable)
  | S1(_, _, disposable, _, _, _, _, _) => disposable
  | S2(_, _, disposable, _, _, _, _, _, _) => disposable;

let asObservable =
  fun
  | Disposed => RxObservable.never
  | Multicast(_, observable, _)
  | S1(_, observable, _, _, _, _, _, _) => observable
  | S2(_, observable, _, _, _, _, _, _, _) => observable;

let dispose = subject => subject |> asDisposable |> RxDisposable.dispose;

let isDisposed = subject => subject |> asDisposable |> RxDisposable.isDisposed;

let notifyComplete = (exn, subscribers) => {
  let currentSubscribers = subscribers^;

  currentSubscribers
  |> RxCopyOnWriteArray.forEach(subscriber =>
       subscriber |> RxSubscriber.complete(~exn?)
     );
};

let completeWithResult = (~exn=?, subject) => {
  let shouldComplete = !isDisposed(subject);

  if (shouldComplete) {
    switch (subject) {
    | Disposed => ()
    | Multicast(subscribers, _, _) => notifyComplete(exn, subscribers)
    | S1(subscribers, _, _, _, onComplete, _, _, ctx0) =>
      onComplete(ctx0, exn);
      notifyComplete(exn, subscribers);
    | S2(subscribers, _, _, _, onComplete, _, _, ctx0, ctx1) =>
      onComplete(ctx0, ctx1, exn);
      notifyComplete(exn, subscribers);
    };

    subject |> dispose;
  };
  shouldComplete;
};

let complete = (~exn=?, subject) =>
  subject |> completeWithResult(~exn?) |> ignore;

let notifyNext = (next, subscribers) => {
  let currentSubscribers = subscribers^;
  currentSubscribers
  |> RxCopyOnWriteArray.forEach(subscriber =>
       subscriber |> RxSubscriber.next(next)
     );
};

let next = (next, subject) =>
  if (!isDisposed(subject)) {
    switch (subject) {
    | Disposed => ()
    | Multicast(subscribers, _, _) => notifyNext(next, subscribers)
    | S1(subscribers, _, _, onNext, _, _, _, ctx0) =>
      onNext(ctx0, next);
      notifyNext(next, subscribers);
    | S2(subscribers, _, _, onNext, _, _, _, ctx0, ctx1) =>
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

let addToSubscribers = (subscribers, subscriber) => {
  subscribers := subscribers^ |> RxCopyOnWriteArray.addLast(subscriber);

  let disposable =
    RxDisposable.create2(subscriberTeardown, subscriber, subscribers);

  subscriber |> RxSubscriber.addDisposable(disposable) |> ignore;
};

let observableSource = (self, subscriber) =>
  if (isDisposed(self^)) {
    subscriber |> RxSubscriber.dispose;
  } else {
    let self = self^;

    switch (self) {
    | Disposed => ()
    | Multicast(subscribers, _, _) =>
      subscriber |> addToSubscribers(subscribers)
    | S1(subscribers, _, _, _, _, onSubscribe, _, ctx0) =>
      onSubscribe(ctx0, subscriber);
      subscriber |> addToSubscribers(subscribers);
    | S2(subscribers, _, _, _, _, onSubscribe, _, ctx0, ctx1) =>
      onSubscribe(ctx0, ctx1, subscriber);
      subscriber |> addToSubscribers(subscribers);
    };
  };

let disposableTeardown = {
  let clearSubscribers = subscribers =>
    subscribers := RxCopyOnWriteArray.empty();

  self =>
    switch (self^) {
    | Disposed => ()
    | Multicast(subscribers, _, _) => clearSubscribers(subscribers)
    | S1(subscribers, _, _, _, _, _, onDispose, ctx0) =>
      clearSubscribers(subscribers);
      onDispose(ctx0);
    | S2(subscribers, _, _, _, _, _, onDispose, ctx0, ctx1) =>
      clearSubscribers(subscribers);
      onDispose(ctx0, ctx1);
    };
};

let createMulticast = () => {
  let self = ref(Disposed);

  let subscribers = ref(RxCopyOnWriteArray.empty());
  let observable = RxObservable.create1(observableSource, self);
  let disposable = RxDisposable.create1(disposableTeardown, self);

  self := Multicast(subscribers, observable, disposable);
  self^;
};

let create1 = (~onNext, ~onComplete, ~onSubscribe, ~onDispose, ctx0) => {
  let self = ref(Disposed);

  let subscribers = ref(RxCopyOnWriteArray.empty());
  let observable = RxObservable.create1(observableSource, self);
  let disposable = RxDisposable.create1(disposableTeardown, self);

  self :=
    S1(
      subscribers,
      observable,
      disposable,
      onNext,
      onComplete,
      onSubscribe,
      onDispose,
      ctx0,
    );
  self^;
};

let create2 = (~onNext, ~onComplete, ~onSubscribe, ~onDispose, ctx0, ctx1) => {
  let self = ref(Disposed);

  let subscribers = ref(RxCopyOnWriteArray.empty());
  let observable = RxObservable.create1(observableSource, self);
  let disposable = RxDisposable.create1(disposableTeardown, self);

  self :=
    S2(
      subscribers,
      observable,
      disposable,
      onNext,
      onComplete,
      onSubscribe,
      onDispose,
      ctx0,
      ctx1,
    );
  self^;
};

let createReplayLast = {
  let onNext = (buffer, next) => buffer := Some(RxNotification.next(next));

  let onComplete = (buffer, exn) =>
    buffer := Some(RxNotification.complete(exn));

  let onSubscribe = {
    let onNext = (subscriber, v) => subscriber |> RxSubscriber.next(v);
    let onComplete = (subscriber, exn) =>
      subscriber |> RxSubscriber.complete(~exn?);

    (buffer, subscriber) =>
      switch (buffer^) {
      | Some(notification) =>
        RxNotification.map1(~onNext, ~onComplete, subscriber, notification)
      | _ => ()
      };
  };

  let onDispose = buffer => buffer := None;

  () => {
    let buffer = ref(None);
    create1(~onNext, ~onComplete, ~onSubscribe, ~onDispose, buffer);
  };
};

let createReplayBuffer = {
  let onNext = (maxBufferCount, buffer, next) => {
    let currentBuffer = buffer^;

    buffer :=
      currentBuffer
      |> RxCopyOnWriteArray.addLastWithMaxCount(
           maxBufferCount,
           RxNotification.next(next),
         );
  };

  let onComplete = (maxBufferCount, buffer, exn) => {
    let currentBuffer = buffer^;

    buffer :=
      currentBuffer
      |> RxCopyOnWriteArray.addLastWithMaxCount(
           maxBufferCount,
           RxNotification.complete(exn),
         );
  };

  let onSubscribe = {
    let onNext = (subscriber, v) => subscriber |> RxSubscriber.next(v);
    let onComplete = (subscriber, exn) =>
      subscriber |> RxSubscriber.complete(~exn?);

    (_, buffer, subscriber) => {
      let currentBuffer = buffer^;
      RxCopyOnWriteArray.forEach(
        RxNotification.map1(~onNext, ~onComplete, subscriber),
        currentBuffer,
      );
    };
  };

  let onDispose = (_, buffer) => buffer := RxCopyOnWriteArray.empty();

  maxBufferCount => {
    RxPreconditions.checkArgument(
      maxBufferCount > 0,
      "ShareWithReplayBufferObservable: maxBufferCount must be greater than 0.",
    );

    let buffer = ref(RxCopyOnWriteArray.empty());
    create2(
      ~onNext,
      ~onComplete,
      ~onSubscribe,
      ~onDispose,
      maxBufferCount,
      buffer,
    );
  };
};
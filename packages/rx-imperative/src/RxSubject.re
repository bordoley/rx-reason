type subscribers('a) = ref(RxCopyOnWriteArray.t(RxSubscriber.t('a)));

type onNext2('a, 'ctx0, 'ctx1) = ('ctx0, 'ctx1, 'a) => unit;
type onComplete2('a, 'ctx0, 'ctx1) = ('ctx0, 'ctx1, option(exn)) => unit;
type onSubscribe2('a, 'ctx0, 'ctx1) =
  ('ctx0, 'ctx1, RxSubscriber.t('a)) => unit;
type onDispose2('ctx0, 'ctx1) = ('ctx0, 'ctx1) => unit;

type t('a) =
  | Disposed
  | Multicast(subscribers('a), RxObservable.t('a), RxDisposable.t)
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
      ): t('a);

let disposed = Disposed;

let asDisposable =
  fun
  | Disposed => RxDisposable.disposed
  | Multicast(_, _, disposable)
  | S2(_, _, disposable, _, _, _, _, _, _) => disposable;

let asObservable =
  fun
  | Disposed => RxObservable.never
  | Multicast(_, observable, _)
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
  let shouldComplete = ! isDisposed(subject);

  if (shouldComplete) {
    switch (subject) {
    | Disposed => ()
    | Multicast(subscribers, _, _) => notifyComplete(exn, subscribers)
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
  if (! isDisposed(subject)) {
    switch (subject) {
    | Disposed => ()
    | Multicast(subscribers, _, _) => notifyNext(next, subscribers)
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
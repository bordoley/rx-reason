type subscribers('a) = ref(RxCopyOnWriteArray.t(RxSubscriber.t('a)));

type t('a) =
  | Disposed
  | Value(ref('a), subscribers('a), RxDisposable.t, RxObservable.t('a));

let asObservable =
  fun
  | Disposed => RxObservable.never
  | Value(_, _, _, observable) => observable;

let asDisposable =
  fun
  | Disposed => RxDisposable.disposed
  | Value(_, _, disposable, _) => disposable;

let dispose = rxValue => rxValue |> asDisposable |> RxDisposable.dispose;

let isDisposed = rxValue => rxValue |> asDisposable |> RxDisposable.isDisposed;

let disposed = Disposed;

let subscriberTeardown = (subscribers, subscriber) => {
  let currentSubscribers = subscribers^;
  subscribers :=
    currentSubscribers
    |> RxCopyOnWriteArray.findAndRemoveReference(subscriber);
};

let observableSource = (value, subscribers, disposable, subscriber) =>
  if (RxDisposable.isDisposed(disposable)) {
    subscriber |> RxSubscriber.dispose;
  } else {
    subscribers := subscribers^ |> RxCopyOnWriteArray.addLast(subscriber);
    let disposable =
      RxDisposable.create2(subscriberTeardown, subscribers, subscriber);

    subscriber |> RxSubscriber.addDisposable(disposable) |> ignore;
    subscriber |> RxSubscriber.next(value^);
  };

let onDispose = subscribers => {
  let currentSubscribers = subscribers^;
  currentSubscribers |> RxCopyOnWriteArray.forEach(RxSubscriber.dispose);
  subscribers := RxCopyOnWriteArray.empty();
};

let create = initialValue => {
  let value = ref(initialValue);
  let subscribers = ref(RxCopyOnWriteArray.empty());
  let disposable = RxDisposable.create1(onDispose, subscribers);
  let observable =
    RxObservable.create3(observableSource, value, subscribers, disposable);

  Value(value, subscribers, disposable, observable);
};

let notify = nextValue =>
  fun
  | Disposed => ()
  | Value(value, subscribers, _, _) => {
      value := nextValue;
      subscribers^
      |> RxCopyOnWriteArray.forEach(RxSubscriber.next(nextValue));
    };

let update = (f, self) =>
  switch (self) {
  | Disposed => ()
  | Value(value, _, _, _) =>
    let nextValue = f(value^);
    notify(nextValue, self);
  };

let update1 = (f, ctx0, self) =>
  switch (self) {
  | Disposed => ()
  | Value(value, _, _, _) =>
    let nextValue = f(ctx0, value^);
    notify(nextValue, self);
  };

let update2 = (f, ctx0, ctx1, self) =>
  switch (self) {
  | Disposed => ()
  | Value(value, _, _, _) =>
    let nextValue = f(ctx0, ctx1, value^);
    notify(nextValue, self);
  };

let update3 = (f, ctx0, ctx1, ctx2, self) =>
  switch (self) {
  | Disposed => ()
  | Value(value, _, _, _) =>
    let nextValue = f(ctx0, ctx1, ctx2, value^);
    notify(nextValue, self);
  };

let update4 = (f, ctx0, ctx1, ctx2, ctx3, self) =>
  switch (self) {
  | Disposed => ()
  | Value(value, _, _, _) =>
    let nextValue = f(ctx0, ctx1, ctx2, ctx3, value^);
    notify(nextValue, self);
  };

let update5 = (f, ctx0, ctx1, ctx2, ctx3, ctx4, self) =>
  switch (self) {
  | Disposed => ()
  | Value(value, _, _, _) =>
    let nextValue = f(ctx0, ctx1, ctx2, ctx3, ctx4, value^);
    notify(nextValue, self);
  };
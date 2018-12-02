type subscribers('a) = ref(RxCopyOnWriteArray.t(RxSubscriber.t('a)));

type t('a) = {
  disposable: RxDisposable.t,
  observable: RxObservable.t('a),
  subscribers: subscribers('a),
  value: ref('a),
};

let asObservable = ({observable}) => observable;

let subscriberTeardown = (subscribers, subscriber) => {
  let currentSubscribers = subscribers^;
  subscribers :=
    currentSubscribers
    |> RxCopyOnWriteArray.findAndRemoveReference(subscriber);
};

let observableSource = (value, subscribers, subscriber) => {
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
  let observable = RxObservable.create2(observableSource, value, subscribers);
  let disposable = RxDisposable.create1(onDispose, subscribers);

  {disposable, observable, subscribers, value};
};

let asDisposable = ({disposable}) => disposable;

let dispose = rxValue => rxValue |> asDisposable |> RxDisposable.dispose;

let isDisposed = rxValue => rxValue |> asDisposable |> RxDisposable.isDisposed;

let raiseIfDisposed = rxValue =>
  rxValue |> asDisposable |> RxDisposable.raiseIfDisposed;

let notify = (value, nextValue, subscribers) => {
  value := nextValue;
  RxCopyOnWriteArray.forEach(RxSubscriber.next(nextValue), subscribers);
};

let update = (f, {subscribers, value}) => {
  let nextValue = f(value^);
  notify(value, nextValue, subscribers^);
};

let update1 = (f, ctx0, {subscribers, value}) => {
  let nextValue = f(ctx0, value^);
  notify(value, nextValue, subscribers^);
};

let update2 = (f, ctx0, ctx1, {subscribers, value}) => {
  let nextValue = f(ctx0, ctx1, value^);
  notify(value, nextValue, subscribers^);
};

let update3 = (f, ctx0, ctx1, ctx2, {subscribers, value}) => {
  let nextValue = f(ctx0, ctx1, ctx2, value^);
  notify(value, nextValue, subscribers^);
};

let update4 = (f, ctx0, ctx1, ctx2, ctx3, {subscribers, value}) => {
  let nextValue = f(ctx0, ctx1, ctx2, ctx3, value^);
  notify(value, nextValue, subscribers^);
};

let update5 = (f, ctx0, ctx1, ctx2, ctx3, ctx4, {subscribers, value}) => {
  let nextValue = f(ctx0, ctx1, ctx2, ctx3, ctx4, value^);
  notify(value, nextValue, subscribers^);
};
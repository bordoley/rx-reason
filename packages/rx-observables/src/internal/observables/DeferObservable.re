let create = {
  let source = (f, subscriber) => {
    let innerSubscription =
      f()
      |> PublishToSubscriberObservable.create(subscriber)
      |> RxObservable.connect;

    subscriber |> RxSubscriber.addDisposable(innerSubscription) |> ignore;
  };

  f => RxObservable.create1(source, f);
};

let create1 = {
  let source = (f, ctx0, subscriber) => {
    let innerSubscription =
      f(ctx0)
      |> PublishToSubscriberObservable.create(subscriber)
      |> RxObservable.connect;

    subscriber |> RxSubscriber.addDisposable(innerSubscription) |> ignore;
  };

  (f, ctx0) => RxObservable.create2(source, f, ctx0);
};

let create2 = {
  let source = (f, ctx0, ctx1, subscriber) => {
    let innerSubscription =
      f(ctx0, ctx1)
      |> PublishToSubscriberObservable.create(subscriber)
      |> RxObservable.connect;

    subscriber |> RxSubscriber.addDisposable(innerSubscription) |> ignore;
  };

  (f, ctx0, ctx1) => RxObservable.create3(source, f, ctx0, ctx1);
};

let create3 = {
  let source = (f, ctx0, ctx1, ctx2, subscriber) => {
    let innerSubscription =
      f(ctx0, ctx1, ctx2)
      |> PublishToSubscriberObservable.create(subscriber)
      |> RxObservable.connect;

    subscriber |> RxSubscriber.addDisposable(innerSubscription) |> ignore;
  };

  (f, ctx0, ctx1, ctx2) => RxObservable.create4(source, f, ctx0, ctx1, ctx2);
};
let create = {
  let source = (createResource, disposeResource, f, subscriber) => {
    let disposable = createResource();
    let innerSubscription =
      f(disposable)
      |> PublishToSubscriberObservable.create(subscriber)
      |> RxObservable.connect;

    subscriber
    |> RxSubscriber.addDisposable(RxDisposable.create1(disposeResource, disposable))
    |> RxSubscriber.addDisposable(innerSubscription)
    |> ignore;
  };

  (~createResource, ~disposeResource, f) =>
    RxObservable.create3(source, createResource, disposeResource, f);
};

let create1 = {
  let source = (createResource, disposeResource, ctx0, f, subscriber) => {
    let disposable = createResource(ctx0);

    let innerSubscription =
      f(disposable)
      |> PublishToSubscriberObservable.create(subscriber)
      |> RxObservable.connect;

    subscriber
    |> RxSubscriber.addDisposable(RxDisposable.create1(disposeResource, disposable))
    |> RxSubscriber.addDisposable(innerSubscription)
    |> ignore;
  };

  (~createResource, ~disposeResource, ctx0, f) =>
    RxObservable.create4(source, createResource, disposeResource, ctx0, f);
};

let create2 = {
  let source = (createResource, disposeResource,  ctx0, ctx1, f, subscriber) => {
    let disposable = createResource(ctx0, ctx1);
    let innerSubscription =
      f(disposable)
      |> PublishToSubscriberObservable.create(subscriber)
      |> RxObservable.connect;

    subscriber
    |> RxSubscriber.addDisposable(RxDisposable.create1(disposeResource, disposable))
    |> RxSubscriber.addDisposable(innerSubscription)
    |> ignore;
  };

  (~createResource, ~disposeResource, ctx0, ctx1, f) =>
    RxObservable.create5(source, createResource, disposeResource, ctx0, ctx1, f);
};
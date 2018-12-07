let source = (f, subscriber) => {
  let innerSubscription =
    f()
    |> PublishToSubscriberObservable.create(subscriber)
    |> RxObservable.connect;

  subscriber |> RxSubscriber.addDisposable(innerSubscription) |> ignore;
};

let create = f => RxObservable.create1(source, f);
let defer = {
  let source = (f, subscriber) => {
    let innerSubscription =
      f()
      |> RxObservable.observe1(
           ~onNext=RxSubscriber.forwardOnNext,
           ~onComplete=RxSubscriber.forwardOnComplete,
           subscriber,
         )
      |> RxObservable.subscribe;

    subscriber
    |> RxSubscriber.addTeardown1(RxDisposable.dispose, innerSubscription)
    |> ignore;
  };

  f => RxObservable.create1(source, f);
};
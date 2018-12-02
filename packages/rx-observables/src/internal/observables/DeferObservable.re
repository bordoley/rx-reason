let defer = {
  let source = (f, subscriber) => {
    let innerSubscription =
      f()
      |> RxObservable.lift(
           ObserveOperator.operator1(
             ~onNext=SubscriberForward.onNext,
             ~onComplete=SubscriberForward.onComplete,
             subscriber,
           ),
         )
      |> RxObservable.subscribe;

    subscriber |> RxSubscriber.addDisposable(innerSubscription) |> ignore;
  };

  f => RxObservable.create1(source, f);
};
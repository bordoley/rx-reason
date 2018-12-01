let subscribeOn = {
  let doSubscribe = (observable, subscriber, ~now as _, ~shouldYield as _) => {
    let innerSubscription =
      observable
      |> RxObservable.observe1(
           ~onNext=SubscriberForward.onNext,
           ~onComplete=SubscriberForward.onComplete,
           subscriber,
         )
      |> RxObservable.subscribe;

    subscriber |> RxSubscriber.addDisposable(innerSubscription) |> ignore;

    RxScheduler.Result.complete;
  };

  let subscribeOnSource = (delay, scheduler, observable, subscriber) => {
    let schedulerSubscription =
      scheduler
      |> RxScheduler.schedule(~delay?, doSubscribe(observable, subscriber));
    subscriber |> RxSubscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (~delay=?, scheduler, observable) =>
    RxObservable.create3(subscribeOnSource, delay, scheduler, observable);
};
let subscribeOn = {
  let doSubscribe = (observable, subscriber, ~now as _, ~shouldYield as _) => {
    let innerSubscription =
      observable
      |> RxObservable.observe1(
           ~onNext=RxSubscriber.forwardOnNext,
           ~onComplete=RxSubscriber.forwardOnComplete,
           subscriber,
         )
      |> RxObservable.subscribe;
    
    subscriber
    |> RxSubscriber.addTeardown1(RxDisposable.dispose, innerSubscription)
    |> ignore;

    RxScheduler.Result.complete;
  };

  let subscribeOnSource = (delay, scheduler, observable, subscriber) => {
    let schedulerSubscription =
      scheduler
      |> RxScheduler.schedule(~delay?, doSubscribe(observable, subscriber));
    subscriber
    |> RxSubscriber.addTeardown1(RxDisposable.dispose, schedulerSubscription)
    |> ignore;
  };

  (~delay=?, scheduler, observable) =>
    RxObservable.create3(subscribeOnSource, delay, scheduler, observable);
};
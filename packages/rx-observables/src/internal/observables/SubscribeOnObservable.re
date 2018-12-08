let doSubscribe = (observable, subscriber, ~now as _, ~shouldYield as _) => {
  let innerSubscription =
    observable
    |> PublishToSubscriberObservable.create(subscriber)
    |> RxObservable.connect;

  subscriber |> RxSubscriber.addDisposable(innerSubscription) |> ignore;

  RxScheduler.Result.complete;
};

let subscribeOnSource = (delay, scheduler, observable, subscriber) => {
  let schedulerSubscription =
    scheduler
    |> RxScheduler.schedule(~delay?, doSubscribe(observable, subscriber));
  subscriber |> RxSubscriber.addDisposable(schedulerSubscription) |> ignore;
};

let create = (~delay=?, scheduler) => {
  switch (delay) {
  | Some(delayInMs) =>
    RxPreconditions.checkArgument(
      delayInMs > 0.0,
      "SubscribeOnObservable: If specified, delay must be greater than 0.0 milliseconds",
    )
  | _ => ()
  };
  
  observable =>
    RxObservable.create3(subscribeOnSource, delay, scheduler, observable);
};
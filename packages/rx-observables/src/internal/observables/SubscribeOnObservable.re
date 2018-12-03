let doSubscribe = (observable, subscriber, ~now as _, ~shouldYield as _) => {
  let innerSubscription =
    observable
    |> RxObservable.lift(ForwardingOperator.create(subscriber))
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

let create = (~delay=?, scheduler, observable) =>
  RxObservable.create3(subscribeOnSource, delay, scheduler, observable);
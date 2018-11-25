let subscribeOn = {
  let doSubscribe = (observable, subscriber, ~now as _, ~shouldYield as _) => {
    observable |> Observable.subscribeWith(subscriber);
    Scheduler.Result.complete;
  };

  let subscribeOnSource = (delay, scheduler, observable, subscriber) => {
    let schedulerSubscription =
      scheduler
      |> Scheduler.schedule(~delay?, doSubscribe(observable, subscriber));
    subscriber
    |> Subscriber.addTeardown1(Disposable.dispose, schedulerSubscription)
    |> ignore;
  };

  (~delay=?, scheduler, observable) =>
    Observable.create3(subscribeOnSource, delay, scheduler, observable);
};
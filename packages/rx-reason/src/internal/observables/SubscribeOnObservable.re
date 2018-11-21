let subscribeOn = {
  let doSubscribe = (observable, subscriber, _) => {
    observable |> Observable.subscribeWith(subscriber);
  };

  let subscribeOnSource = (delay, scheduler, observable, subscriber) => {
    let schedulerSubscription =
      scheduler
      |> Scheduler.scheduleAfter2(
           ~delay,
           doSubscribe,
           (),
           observable,
           subscriber,
         );
    subscriber
    |> Subscriber.addTeardown1(Disposable.dispose, schedulerSubscription)
    |> ignore;
  };

  (~delay=0.0, scheduler, observable) =>
    Observable.create3(subscribeOnSource, delay, scheduler, observable);
};
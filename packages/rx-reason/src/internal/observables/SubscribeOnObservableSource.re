let subscribeOn = {
  let doSubscribe = (observable, subscriber, _) => {
    observable |> ObservableSource.subscribeSubscriber(subscriber);
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
    ObservableSource.create3(subscribeOnSource, delay, scheduler, observable);
};
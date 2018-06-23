let subscribeOn = {
  let doSubscribe = (observable, subscriber, _, _) =>
    observable |> ObservableSource.subscribeSubscriber(subscriber);

  let subscribeOnSource = (scheduler, observable, subscriber) => {
    let schedulerSubscription =
      scheduler
      |> SchedulerNew.schedule2(doSubscribe, (), observable, subscriber);
    subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (scheduler, observable) =>
    ObservableSource.create2(subscribeOnSource, scheduler, observable);
};
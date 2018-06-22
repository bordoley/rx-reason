let subscribeOn = {
  let subscribeOnSource = (scheduler, observable, subscriber) => {
    let schedulerSubscription =
      scheduler(() => {
        observable |> ObservableSource.subscribeSubscriber(subscriber);
        Disposable.disposed;
      });
    subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (scheduler, observable) =>
    ObservableSource.create2(subscribeOnSource, scheduler, observable);
};
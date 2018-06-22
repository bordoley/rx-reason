let ofValue = {
  let ofValueSynchronousSource = (value, subscriber) => {
    subscriber |> Subscriber.next(value);
    subscriber |> Subscriber.complete;
  };

  let ofValueScheduledSource = (scheduler, value, subscriber) => {
    let schedulerSubscription =
      scheduler(() => {
        subscriber |> Subscriber.next(value);
        scheduler(() => {
          subscriber |> Subscriber.complete;
          Disposable.disposed;
        });
      });

    subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (~scheduler=Scheduler.immediate, value) =>
    scheduler === Scheduler.immediate ?
      ObservableSource.create1(ofValueSynchronousSource, value) :
      ObservableSource.create2(ofValueScheduledSource, scheduler, value);
};
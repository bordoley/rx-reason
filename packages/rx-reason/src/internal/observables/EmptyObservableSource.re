let empty = {
  let emptySynchronousSource = subscriber => subscriber |> Subscriber.complete;

  let emptyScheduledSource = (scheduler, subscriber) => {
    let schedulerSubscription =
      scheduler(() => {
        subscriber |> Subscriber.complete;
        Disposable.disposed;
      });
    subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (~scheduler=Scheduler.immediate, ()) =>
    scheduler === Scheduler.immediate ?
      ObservableSource.create(emptySynchronousSource) :
      ObservableSource.create1(emptyScheduledSource, scheduler);
};
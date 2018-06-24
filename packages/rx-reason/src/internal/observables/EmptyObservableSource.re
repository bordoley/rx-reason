let empty = {
  let emptySynchronousSource = subscriber => subscriber |> Subscriber.complete;

  let emptyScheduledSource = {
    let doWork = (subscriber, continuation, ()) => {
      subscriber |> Subscriber.complete;
      continuation |> SchedulerContinuation.dispose;
    };

    (scheduler, subscriber) => {
      let schedulerSubscription =
        scheduler |> Scheduler.schedule1(doWork, (), subscriber);
      subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
    };
  };

  (~scheduler=Scheduler.immediate, ()) =>
    scheduler === Scheduler.immediate ?
      ObservableSource.create(emptySynchronousSource) :
      ObservableSource.create1(emptyScheduledSource, scheduler);
};
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
      subscriber
      |> Subscriber.addTeardown1(Disposable.dispose, schedulerSubscription)
      |> ignore;
    };
  };

  (~scheduler=?, ()) =>
    switch (scheduler) {
    | Some(scheduler) =>
      ObservableSource.create1(emptyScheduledSource, scheduler)
    | None => ObservableSource.create(emptySynchronousSource)
    };
};
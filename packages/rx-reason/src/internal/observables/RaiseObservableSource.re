let raise = {
  let raiseSynchronousSource = (exn, subscriber) =>
    subscriber |> Subscriber.complete(~exn?);

  let raiseScheduledSource = {
    let doWork = (subscriber, exn, continuation, ()) => {
      subscriber |> Subscriber.complete(~exn?);
      continuation |> SchedulerContinuation.dispose;
    };

    (scheduler, exn, subscriber) => {
      let schedulerSubscription =
        scheduler |> Scheduler.schedule2(doWork, (), subscriber, exn);
      subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
    };
  };

  (~scheduler=?, exn: exn) => {
    let exn = Some(exn);

    switch (scheduler) {
    | Some(scheduler) =>
      ObservableSource.create2(raiseScheduledSource, scheduler, exn)
    | None => ObservableSource.create1(raiseSynchronousSource, exn)
    };
  };
};
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

  (~scheduler=Scheduler.immediate, exn: exn) => {
    let exn = Some(exn);

    scheduler === Scheduler.immediate ?
      ObservableSource.create1(raiseSynchronousSource, exn) :
      ObservableSource.create2(raiseScheduledSource, scheduler, exn);
  };
};
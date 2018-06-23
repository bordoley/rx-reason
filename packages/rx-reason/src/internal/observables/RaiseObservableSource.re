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
        scheduler |> SchedulerNew.schedule2(doWork, (), subscriber, exn);
      subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
    };
  };

  (~scheduler=SchedulerNew.immediate, exn: exn) => {
    let exn = Some(exn);

    scheduler === SchedulerNew.immediate ?
      ObservableSource.create1(raiseSynchronousSource, exn) :
      ObservableSource.create2(raiseScheduledSource, scheduler, exn);
  };
};
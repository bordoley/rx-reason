let raise = {
  let sourceSynchronous = (exn, subscriber) =>
    subscriber |> Subscriber.complete(~exn?);

  let sourceScheduled = (scheduler, exn, subscriber) => {
    let schedulerDisposable =
          scheduler(() => {
            subscriber |> Subscriber.complete(~exn?);
            Disposable.disposed;
          });
        subscriber |> Subscriber.addDisposable(schedulerDisposable) |> ignore;
  };

  (~scheduler=Scheduler.immediate, exn: exn) => {
    let exn = Some(exn);

    scheduler === Scheduler.immediate ?
      ObservableSource.create1(sourceSynchronous, exn) :
      ObservableSource.create2(sourceScheduled, scheduler, exn);
  };
};
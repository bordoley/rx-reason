let raise = {
  let raiseSynchronousSource = (exn, subscriber) =>
    subscriber |> Subscriber.complete(~exn?);

  (~scheduler=?, exn: exn) => {
    let exn = Some(exn);
    let source = ObservableSource.create1(raiseSynchronousSource, exn);

    switch (scheduler) {
    | Some(scheduler) =>
      source |> SubscribeOnObservableSource.subscribeOn(scheduler);
    | None => source;
    };
  };
};
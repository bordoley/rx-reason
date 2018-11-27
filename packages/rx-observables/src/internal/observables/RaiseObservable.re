let raise = {
  let raiseSynchronousSource = (exn, subscriber) =>
    subscriber |> RxSubscriber.complete(~exn?);

  (~scheduler=?, exn) => {
    let exn = Some(exn);
    let source = RxObservable.create1(raiseSynchronousSource, exn);

    switch (scheduler) {
    | Some(scheduler) =>
      source |> SubscribeOnObservable.subscribeOn(scheduler);
    | None => source;
    };
  };
};
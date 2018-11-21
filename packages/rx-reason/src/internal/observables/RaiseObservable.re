let raise = {
  let raiseSynchronousSource = (exn, subscriber) =>
    subscriber |> Subscriber.complete(~exn?);

  (~scheduler=?, exn) => {
    let exn = Some(exn);
    let source = Observable.create1(raiseSynchronousSource, exn);

    switch (scheduler) {
    | Some(scheduler) =>
      source |> SubscribeOnObservable.subscribeOn(scheduler);
    | None => source;
    };
  };
};
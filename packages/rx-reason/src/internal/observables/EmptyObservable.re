let empty = {
  let emptySynchronousSource = (subscriber) =>
    subscriber |> Subscriber.complete;

  (~scheduler=?, ()) => {
    let source = Observable.create(emptySynchronousSource);

    switch (scheduler) {
    | Some(scheduler) =>
      source |> SubscribeOnObservable.subscribeOn(scheduler);
    | None => source;
    };
  };
};
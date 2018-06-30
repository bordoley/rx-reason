let empty = {
  let emptySynchronousSource = (subscriber) =>
    subscriber |> Subscriber.complete;

  (~scheduler=?, ()) => {
    let source = ObservableSource.create(emptySynchronousSource);

    switch (scheduler) {
    | Some(scheduler) =>
      source |> SubscribeOnObservableSource.subscribeOn(scheduler);
    | None => source;
    };
  };
};
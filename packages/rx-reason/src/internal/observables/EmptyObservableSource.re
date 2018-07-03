let empty = {
  let emptySynchronousSource = (_, subscriber) =>
    subscriber |> Subscriber.complete;

  (~scheduler=?, ()) => {
    let source = ObservableSource.create(emptySynchronousSource);

    switch (scheduler) {
    | Some(scheduler) =>
      source |> SubscribeOnObservableSource.subscribeOn(scheduler)
    | None => source
    };
  };
};
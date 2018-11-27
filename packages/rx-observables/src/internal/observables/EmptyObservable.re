let empty = {
  let emptySynchronousSource = (subscriber) =>
    subscriber |> RxSubscriber.complete;

  (~scheduler=?, ()) => {
    let source = RxObservable.create(emptySynchronousSource);

    switch (scheduler) {
    | Some(scheduler) =>
      source |> SubscribeOnObservable.subscribeOn(scheduler);
    | None => source;
    };
  };
};
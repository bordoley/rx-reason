let ofValueSynchronousSource = (value, subscriber) => {
  subscriber |> RxSubscriber.next(value);
  subscriber |> RxSubscriber.complete;
};

let create = (~scheduler=?, value) => {
  let source = RxObservable.create1(ofValueSynchronousSource, value);

  switch (scheduler) {
  | Some(scheduler) => source |> SubscribeOnObservable.create(scheduler)
  | None => source
  };
};
let raiseSynchronousSource = (exn, subscriber) =>
  subscriber |> RxSubscriber.complete(~exn?);

let create = (~scheduler=?, exn) => {
  let exn = Some(exn);
  let source = RxObservable.create1(raiseSynchronousSource, exn);

  switch (scheduler) {
  | Some(scheduler) => source |> SubscribeOnObservable.create(scheduler)
  | None => source
  };
};
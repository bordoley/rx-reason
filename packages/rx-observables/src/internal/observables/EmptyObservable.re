let emptySynchronousSource = subscriber => subscriber |> RxSubscriber.complete;

let create = (~scheduler=?, ()) => {
  let source = RxObservable.create(emptySynchronousSource);

  switch (scheduler) {
  | Some(scheduler) => source |> SubscribeOnObservable.create(scheduler)
  | None => source
  };
};
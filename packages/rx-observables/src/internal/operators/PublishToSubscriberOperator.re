let onNext = (subscriber, v) => subscriber |> RxSubscriber.next(v);

let onComplete = (subscriber, exn) =>
  subscriber |> RxSubscriber.complete(~exn?);

let create = subscriber =>
  ObserveOperator.create1(~onNext, ~onComplete, subscriber);
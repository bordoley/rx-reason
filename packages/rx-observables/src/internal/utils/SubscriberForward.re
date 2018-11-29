let onComplete = (subscriber, exn) => subscriber |> RxSubscriber.complete(~exn?);

let onComplete1 = (_, subscriber, exn) =>
  subscriber |> RxSubscriber.complete(~exn?);

let onComplete2 = (_, _, subscriber, exn) =>
  subscriber |> RxSubscriber.complete(~exn?);

let onComplete3 = (_, _, _, subscriber, exn) =>
  subscriber |> RxSubscriber.complete(~exn?);

let onComplete4 = (_, _, _, _, subscriber, exn) =>
  subscriber |> RxSubscriber.complete(~exn?);

let onComplete5 = (_, _, _, _, _, subscriber, exn) =>
  subscriber |> RxSubscriber.complete(~exn?);

let onNext = (subscriber, v) => subscriber |> RxSubscriber.next(v);

let onNext1 = (_, subscriber, v) => subscriber |> RxSubscriber.next(v);

let onNext2 = (_, _, subscriber, v) => subscriber |> RxSubscriber.next(v);

let onNext3 = (_, _, _, subscriber, v) => subscriber |> RxSubscriber.next(v);

let onNext4 = (_, _, _, _, subscriber, v) => subscriber |> RxSubscriber.next(v);

let onNext5 = (_, _, _, _, _, subscriber, v) => subscriber |> RxSubscriber.next(v);
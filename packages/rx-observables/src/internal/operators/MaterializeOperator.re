let onNext = (delegate, next) =>
  delegate |> RxSubscriber.next(RxNotification.next(next));

let onComplete = (delegate, exn) => {
  let next = RxNotification.complete(exn);
  delegate |> RxSubscriber.next(next);
  delegate |> RxSubscriber.complete;
};

let create = subscriber =>
  subscriber |> RxSubscriber.decorate(~onNext, ~onComplete);
let operator = {
  let onNext = (delegate, next) =>
    delegate |> RxSubscriber.next(RxNotification.Next(next));

  let onComplete = (delegate, exn) => {
    let next = RxNotification.complete(exn);
    delegate |> RxSubscriber.next(next);
    delegate |> RxSubscriber.complete;
  };

  subscriber => subscriber |> RxSubscriber.decorate(~onNext, ~onComplete);
};
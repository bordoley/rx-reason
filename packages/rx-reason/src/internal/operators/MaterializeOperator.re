let operator = {
  let onNext = (delegate, next) =>
    delegate |> Subscriber.next(Notification.Next(next));

  let onComplete = (delegate, exn) => {
    let next = Notification.complete(exn)
    delegate |> Subscriber.next(next);
    delegate |> Subscriber.complete;
  };

  subscriber => subscriber |> Subscriber.decorate(~onNext, ~onComplete);
};
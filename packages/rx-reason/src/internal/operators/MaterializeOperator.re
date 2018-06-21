let operator = {
  let onNext = (delegate, next) =>
    delegate |> Subscriber.next(Notification.Next(next));

  let onComplete = (delegate, exn) => {
    let next =
      switch (exn) {
      | Some(exn) => Notification.CompleteWithException(exn)
      | None => Notification.Complete
      };
    delegate |> Subscriber.next(next);
    delegate |> Subscriber.complete;
  };

  subscriber => subscriber |> Subscriber.delegate(~onNext, ~onComplete);
};

let lift = observable => observable |> ObservableSource.lift(operator);
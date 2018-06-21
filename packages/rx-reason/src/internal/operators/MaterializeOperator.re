let operator = {
  let onNext = (_, delegate, next) =>
    delegate |> Subscriber.next(Notification.Next(next));

  let onComplete = (_, delegate, exn) => {
    let next =
      switch (exn) {
      | Some(exn) => Notification.CompleteWithException(exn)
      | None => Notification.Complete
      };
    delegate |> Subscriber.next(next);
    delegate |> Subscriber.complete;
  };

  subscriber => subscriber |> Subscriber.delegate(~onNext, ~onComplete, None);
};

let lift = observable => observable |> ObservableSource.lift(operator);
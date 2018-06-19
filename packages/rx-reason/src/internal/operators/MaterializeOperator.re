let operator = subscriber => {
  let onNext = next => subscriber |> Subscriber.next(Notification.Next(next));

  let onComplete = exn => {
    let next =
      switch (exn) {
      | Some(exn) => Notification.CompleteWithException(exn)
      | None => Notification.Complete
      };
    subscriber |> Subscriber.next(next);
    subscriber |> Subscriber.complete;
  };
  
  subscriber |> Subscriber.delegate(~onNext, ~onComplete);
};

let lift = observable => observable |> ObservableSource.lift(operator);
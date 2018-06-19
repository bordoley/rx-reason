let operator = observer => {
  let onNext = next => observer |> Observer.next(Notification.Next(next));

  let onComplete = exn => {
    let next =
      switch (exn) {
      | Some(exn) => Notification.CompleteWithException(exn)
      | None => Notification.Complete
      };
    observer |> Observer.next(next);
    observer |> Observer.complete;
  };
  
  observer |> Observer.delegate(~onNext, ~onComplete);
};

let lift = observable => observable |> ObservableSource.lift(operator);
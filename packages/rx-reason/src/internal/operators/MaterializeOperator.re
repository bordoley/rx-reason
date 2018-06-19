let operator = observer =>
  Observer.delegate(
    ~onNext=next => observer |> Observer.next(Notification.Next(next)),
    ~onComplete=
      exn => {
        let next =
          switch (exn) {
          | Some(exn) => Notification.CompleteWithException(exn)
          | None => Notification.Complete
          };
        observer |> Observer.next(next);
        observer |> Observer.complete;
      },
    observer,
  );

let lift = observable => observable |> ObservableSource.lift(operator);
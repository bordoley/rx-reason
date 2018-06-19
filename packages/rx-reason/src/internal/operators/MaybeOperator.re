let operator = observer =>
  Observer.delegate(
    ~onNext=Observer.forwardOnNext(observer),
    ~onComplete=
      exn => {
        let exn =
          switch (exn) {
          | Some(EmptyException.EmptyException) => None
          | _ => exn
          };
        observer |> Observer.complete(~exn?);
      },
    observer,
  );

let lift = observable => observable |> ObservableSource.lift(operator);
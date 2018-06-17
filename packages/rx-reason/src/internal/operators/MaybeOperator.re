let operator = observer =>
  Observer.create(
    ~onNext=Observer.forwardOnNext(observer),
    ~onComplete=
      exn => {
        let exn =
          switch (exn) {
          | Some(Exceptions.EmptyException) => None
          | _ => exn
          };
        observer |> Observer.complete(~exn?);
      },
    ~onDispose=Observer.forwardOnDispose(observer),
  );

let lift = observable => observable |> ObservableSource.lift(operator);
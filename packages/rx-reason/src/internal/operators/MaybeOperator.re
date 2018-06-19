let operator = observer => {
  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(EmptyException.Exn) => None
      | _ => exn
      };
    observer |> Observer.complete(~exn?);
  };
  observer
  |> Observer.delegate(~onNext=Observer.forwardOnNext(observer), ~onComplete);
};

let lift = observable => observable |> ObservableSource.lift(operator);
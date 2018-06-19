let operator = observer => {
  let last = MutableOption.create();

  let onNext = next => MutableOption.set(next, last);
  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(_) => exn
      | None =>
        if (MutableOption.isEmpty(last)) {
          Some(EmptyException.Exn);
        } else {
          let lastValue = MutableOption.get(last);
          observer |> Observer.next(lastValue);
          None;
        }
      };
    observer |> Observer.complete(~exn?);
  };
  
  observer
  |> Observer.delegate(~onNext, ~onComplete)
  |> Observer.addTeardown(() => MutableOption.unset(last));
};

let lift = observable => observable |> ObservableSource.lift(operator);
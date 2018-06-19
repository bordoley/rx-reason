let operator = (default, observer) => {
  let isEmpty = ref(true);

  let onNext = next => {
    observer |> Observer.next(next);
    Volatile.write(false, isEmpty);
  };

  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(EmptyException.Exn)
      | None =>
        if (Volatile.read(isEmpty)) {
          observer |> Observer.next(default);
        };
        None;
      | Some(_) => exn
      };
    observer |> Observer.complete(~exn?);
  };
  
  observer |> Observer.delegate(~onNext, ~onComplete);
};

let lift = (default, observable) =>
  observable |> ObservableSource.lift(operator(default));
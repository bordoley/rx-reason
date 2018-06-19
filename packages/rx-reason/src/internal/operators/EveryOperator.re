exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator = (predicate, observer) => {
  let everyTrueObserver = ref(Observer.disposed);

  let onNext = next =>
    if (! next) {
      everyTrueObserver^ |> Observer.complete(~exn=?completeWithoutErrorExn);
    };

  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) =>
        observer |> Observer.next(false);
        None;
      | None =>
        observer |> Observer.next(true);
        None;
      | _ => exn
      };
    observer |> Observer.complete(~exn?);
  };

  everyTrueObserver := observer |> Observer.delegate(~onNext, ~onComplete);
  everyTrueObserver^ |> MapOperator.operator(predicate);
};

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));
exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator = observer => {
  let firstObserver = ref(Observer.disposed);

  let onNext = next => {
    observer |> Observer.next(next);
    firstObserver^ |> Observer.complete(~exn=?completeWithoutErrorExn);
  };

  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) => None
      | Some(_) => exn
      | _ => Some(EmptyException.Exn)
      };
    observer |> Observer.complete(~exn?);
  };

  firstObserver := observer |> Observer.delegate(~onNext, ~onComplete);
  firstObserver^;
};

let lift = observable => observable |> ObservableSource.lift(operator);
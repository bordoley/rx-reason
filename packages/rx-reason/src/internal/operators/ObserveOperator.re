let operator = (~onNext, ~onComplete, observer) => {
  let observeObserver = ref(Observer.disposed);
  
  let onNext =
    Functions.earlyReturnsUnit1(next => {
      try (onNext(next)) {
      | exn =>
        observeObserver^ |> Observer.complete(~exn);
        Functions.returnUnit();
      };
      observer |> Observer.next(next);
    });

  let onComplete = exn => {
    let exn =
      try (
        {
          onComplete(exn);
          exn;
        }
      ) {
      | exn => Some(exn)
      };
    observer |> Observer.complete(~exn?);
  };

  observeObserver := observer |> Observer.delegate(~onNext, ~onComplete);
  observeObserver^;
};

let lift = (~onNext, ~onComplete, observable) =>
  observable |> ObservableSource.lift(operator(~onNext, ~onComplete));
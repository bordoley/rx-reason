let operator = (~onNext, ~onComplete, observer) => {
  let observeObserver = ref(Observer.disposed);
  observeObserver :=
    Observer.delegate(
      ~onNext=
        Functions.earlyReturnsUnit1(next => {
          try (onNext(next)) {
          | exn =>
            observeObserver^ |> Observer.complete(~exn);
            Functions.returnUnit();
          };
          observer |> Observer.next(next);
        }),
      ~onComplete=
        exn => {
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
        },
      observer,
    );
  observeObserver^;
};

let lift = (~onNext, ~onComplete, observable) =>
  observable |> ObservableSource.lift(operator(~onNext, ~onComplete));
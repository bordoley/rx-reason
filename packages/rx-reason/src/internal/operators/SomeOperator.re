exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator = (predicate, observer) => {
  let someTrueObserver = ref(Observer.disposed);
  someTrueObserver :=
    Observer.delegate(
      ~onNext=
        next =>
          if (next) {
            someTrueObserver^
            |> Observer.complete(~exn=?completeWithoutErrorExn);
          },
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(CompleteWithoutErrorException) =>
              observer |> Observer.next(true);
              None;
            | None =>
              observer |> Observer.next(false);
              None;
            | _ => exn
            };
          observer |> Observer.complete(~exn?);
        },
      observer,
    );
  someTrueObserver^ |> MapOperator.operator(predicate);
};

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));
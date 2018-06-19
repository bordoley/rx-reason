exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator = (predicate, observer) => {
  let everyTrueObserver = ref(Observer.disposed);
  everyTrueObserver :=
    Observer.delegate(
      ~onNext=
        next =>
          if (! next) {
            everyTrueObserver^
            |> Observer.complete(~exn=?completeWithoutErrorExn);
          },
      ~onComplete=
        exn => {
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
        },
      observer,
    );
  everyTrueObserver^ |> MapOperator.operator(predicate);
};

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));
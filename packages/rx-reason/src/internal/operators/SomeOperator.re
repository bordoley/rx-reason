exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator = (predicate, subscriber) => {
  let someTrueSubscriber = ref(Subscriber.disposed);

  let onNext = next =>
    if (next) {
      someTrueSubscriber^ |> Subscriber.complete(~exn=?completeWithoutErrorExn);
    };

  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) =>
        subscriber |> Subscriber.next(true);
        None;
      | None =>
        subscriber |> Subscriber.next(false);
        None;
      | _ => exn
      };
    subscriber |> Subscriber.complete(~exn?);
  };

  someTrueSubscriber := subscriber |> Subscriber.delegate(~onNext, ~onComplete);
  someTrueSubscriber^ |> MapOperator.operator(predicate);
};

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));
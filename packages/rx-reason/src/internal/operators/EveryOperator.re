exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator = (predicate, subscriber) => {
  let everyTrueSubscriber = ref(Subscriber.disposed);

  let onNext = next =>
    if (! next) {
      everyTrueSubscriber^ |> Subscriber.complete(~exn=?completeWithoutErrorExn);
    };

  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) =>
        subscriber |> Subscriber.next(false);
        None;
      | None =>
        subscriber |> Subscriber.next(true);
        None;
      | _ => exn
      };
    subscriber |> Subscriber.complete(~exn?);
  };

  everyTrueSubscriber := subscriber |> Subscriber.delegate(~onNext, ~onComplete);
  everyTrueSubscriber^ |> MapOperator.operator(predicate);
};

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));
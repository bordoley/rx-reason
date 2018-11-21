exception CompleteWithoutErrorException;

let operator = {
  let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

  let onNext = (self, _, next) =>
    if (! next) {
      self^ |> Subscriber.complete(~exn=?completeWithoutErrorExn);
    };

  let onComplete = (_, delegate, exn) => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) =>
        delegate |> Subscriber.next(false);
        None;
      | None =>
        delegate |> Subscriber.next(true);
        None;
      | _ => exn
      };
    delegate |> Subscriber.complete(~exn?);
  };

  (predicate, subscriber) => {
    let self = ref(Subscriber.disposed);
    self := subscriber |> Subscriber.decorate1(~onNext, ~onComplete, self);
    self^ |> MapOperator.operator(predicate);
  };
};
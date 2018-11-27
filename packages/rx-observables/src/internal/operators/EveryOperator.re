exception CompleteWithoutErrorException;

let operator = {
  let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

  let onNext = (self, _, next) =>
    if (! next) {
      self^ |> RxSubscriber.complete(~exn=?completeWithoutErrorExn);
    };

  let onComplete = (_, delegate, exn) => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) =>
        delegate |> RxSubscriber.next(false);
        None;
      | None =>
        delegate |> RxSubscriber.next(true);
        None;
      | _ => exn
      };
    delegate |> RxSubscriber.complete(~exn?);
  };

  (predicate, subscriber) => {
    let self = ref(RxSubscriber.disposed);
    self := subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, self);
    self^ |> MapOperator.operator(predicate);
  };
};
exception CompleteWithoutErrorException;

let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let create = predicate => {
  let onNext =
    (. self, _, next) =>
      if (next) {
        self^ |> RxSubscriber.complete(~exn=?completeWithoutErrorExn);
      };

  let onComplete =
    (. _, delegate, exn) => {
      let exn =
        switch (exn) {
        | Some(CompleteWithoutErrorException) =>
          delegate |> RxSubscriber.next(true);
          None;
        | None =>
          delegate |> RxSubscriber.next(false);
          None;
        | _ => exn
        };
      delegate |> RxSubscriber.complete(~exn?);
    };
    
  subscriber => {
    let self = ref(RxSubscriber.disposed);
    self := subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, self);
    self^ |> MapOperator.create(predicate);
  };
};
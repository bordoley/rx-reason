exception CompleteWithoutErrorException;

let operator = {
  let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

  let onNext = (self, delegate, _) => {
    delegate |> RxSubscriber.next(false);
    self^ |> RxSubscriber.complete(~exn=?completeWithoutErrorExn);
  };

  let onComplete = (_, delegate, exn) => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) => None
      | Some(_) => exn
      | None =>
        delegate |> RxSubscriber.next(true);
        exn;
      };
    delegate |> RxSubscriber.complete(~exn?);
  };

  subscriber => {
    let self = ref(RxSubscriber.disposed);
    self := subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, self);
    self^;
  };
};
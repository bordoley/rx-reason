exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator = subscriber => {
  let isEmptySubscriber = ref(Subscriber.disposed);

  let onNext = _ => {
    subscriber |> Subscriber.next(false);
    isEmptySubscriber^ |> Subscriber.complete(~exn=?completeWithoutErrorExn);
  };

  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) => None
      | Some(_) => exn
      | None =>
        subscriber |> Subscriber.next(true);
        exn;
      };
    subscriber |> Subscriber.complete(~exn?);
  };
  isEmptySubscriber := subscriber |> Subscriber.delegate(~onNext, ~onComplete);
  isEmptySubscriber^;
};

let lift = observable => observable |> ObservableSource.lift(operator);
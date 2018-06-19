exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator = subscriber => {
  let firstSubscriber = ref(Subscriber.disposed);

  let onNext = next => {
    subscriber |> Subscriber.next(next);
    firstSubscriber^ |> Subscriber.complete(~exn=?completeWithoutErrorExn);
  };

  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) => None
      | Some(_) => exn
      | _ => Some(EmptyException.Exn)
      };
    subscriber |> Subscriber.complete(~exn?);
  };

  firstSubscriber := subscriber |> Subscriber.delegate(~onNext, ~onComplete);
  firstSubscriber^;
};

let lift = observable => observable |> ObservableSource.lift(operator);
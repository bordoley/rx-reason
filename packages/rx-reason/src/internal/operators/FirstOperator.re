exception CompleteWithoutErrorException;

let operator = {
  let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

  let onNext = (self, subscriber, next) => {
    subscriber |> Subscriber.next(next);
    self^ |> Subscriber.complete(~exn=?completeWithoutErrorExn);
  };

  let onComplete = (_, subscriber, exn) => {
    let exn =
      switch (exn) {
      | Some(CompleteWithoutErrorException) => None
      | Some(_) => exn
      | _ => Some(EmptyException.Exn)
      };
    subscriber |> Subscriber.complete(~exn?);
  };

  subscriber => {
    let self = ref(Subscriber.disposed);
    self := subscriber |> Subscriber.delegate1(~onNext, ~onComplete, self);
    self^;
  };
};

let lift = observable => observable |> ObservableSource.lift(operator);
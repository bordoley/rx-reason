exception CompleteWithoutErrorException;

let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let onNext = (self, subscriber, next) => {
  subscriber |> RxSubscriber.next(next);
  self^ |> RxSubscriber.complete(~exn=?completeWithoutErrorExn);
};

let onComplete = (_, subscriber, exn) => {
  let exn =
    switch (exn) {
    | Some(CompleteWithoutErrorException) => None
    | Some(_) => exn
    | _ => Some(RxEmptyException.Exn)
    };
  subscriber |> RxSubscriber.complete(~exn?);
};

let create = subscriber => {
  let self = ref(RxSubscriber.disposed);
  self := subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, self);
  self^;
};
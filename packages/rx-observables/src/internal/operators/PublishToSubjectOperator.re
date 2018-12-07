let onComplete = (subject, exn) => subject |> RxSubject.complete(~exn?);

let onNext = (subject, v) => subject |> RxSubject.next(v);

let create = subject =>
  ObserveOperator.create1(~onNext, ~onComplete, subject);
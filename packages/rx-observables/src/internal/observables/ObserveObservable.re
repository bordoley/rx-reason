let create = (~onNext, ~onComplete, observable) =>
  observable
  |> RxObservable.lift(ObserveOperator.create(~onNext, ~onComplete));

let create1 = (~onNext, ~onComplete, ctx0, observable) =>
  observable
  |> RxObservable.lift(ObserveOperator.create1(~onNext, ~onComplete, ctx0));

let create2 = (~onNext, ~onComplete, ctx0, ctx1, observable) =>
  observable
  |> RxObservable.lift(
       ObserveOperator.create2(~onNext, ~onComplete, ctx0, ctx1),
     );

let create3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, observable) =>
  observable
  |> RxObservable.lift(
       ObserveOperator.create3(~onNext, ~onComplete, ctx0, ctx1, ctx2),
     );
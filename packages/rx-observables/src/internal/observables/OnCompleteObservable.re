let create = (onComplete, observable) =>
  observable |> RxObservable.lift(OnCompleteOperator.create(onComplete));

let create1 = (onComplete, ctx0, observable) =>
  observable
  |> RxObservable.lift(OnCompleteOperator.create1(onComplete, ctx0));

let create2 = (onComplete, ctx0, ctx1, observable) =>
  observable
  |> RxObservable.lift(OnCompleteOperator.create2(onComplete, ctx0, ctx1));
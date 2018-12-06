let create = (f, observable) =>
  observable |> RxObservable.lift(ExhaustMapOperator.create(f));

let create1 = (f, ctx0, observable) =>
  observable |> RxObservable.lift(ExhaustMapOperator.create1(f, ctx0));
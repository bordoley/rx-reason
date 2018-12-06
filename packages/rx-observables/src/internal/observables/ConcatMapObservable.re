let create = (f, observable) =>
  observable |> RxObservable.lift(ConcatMapOperator.create(f));

let create1 = (f, ctx0, observable) =>
  observable |> RxObservable.lift(ConcatMapOperator.create1(f, ctx0));
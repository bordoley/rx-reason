let create = (f, observable) =>
  observable |> RxObservable.lift(ConcatMapOperator.create(f));

let create1 = (f, ctx0, observable) =>
  observable |> RxObservable.lift(ConcatMapOperator.create1(f, ctx0));

let create2 = (f, ctx0, ctx1, observable) =>
  observable |> RxObservable.lift(ConcatMapOperator.create2(f, ctx0, ctx1));

let create3 = (f, ctx0, ctx1, ctx2, observable) =>
  observable
  |> RxObservable.lift(ConcatMapOperator.create3(f, ctx0, ctx1, ctx2));
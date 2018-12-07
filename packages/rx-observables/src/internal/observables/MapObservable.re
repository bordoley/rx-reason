let create = (f, observable) =>
  observable |> RxObservable.lift(MapOperator.create(f));

let create1 = (f, ctx0, observable) =>
  observable |> RxObservable.lift(MapOperator.create1(f, ctx0));

let create2 = (f, ctx0, ctx1, observable) =>
  observable |> RxObservable.lift(MapOperator.create2(f, ctx0, ctx1));
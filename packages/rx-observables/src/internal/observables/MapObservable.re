let create = (f, observable) =>
  observable |> RxObservable.lift(MapOperator.create(f));

let create1 = (f, ctx0, observable) =>
  observable |> RxObservable.lift(MapOperator.create1(f, ctx0));
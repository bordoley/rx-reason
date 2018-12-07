let create = (f, observable) =>
  observable |> RxObservable.lift(SwitchMapOperator.create(f));

let create1 = (f, ctx0, observable) =>
  observable |> RxObservable.lift(SwitchMapOperator.create1(f, ctx0));

let create2 = (f, ctx0, ctx1, observable) =>
  observable |> RxObservable.lift(SwitchMapOperator.create2(f, ctx0, ctx1));

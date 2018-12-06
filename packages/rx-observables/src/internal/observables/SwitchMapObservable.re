let create = (f, observable) =>
  observable |> RxObservable.lift(SwitchMapOperator.create(f));

let create1 = (f, ctx0, observable) =>
  observable |> RxObservable.lift(SwitchMapOperator.create1(f, ctx0));

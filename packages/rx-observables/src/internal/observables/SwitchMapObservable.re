let create = (f, observable) =>
  observable |> RxObservable.lift(SwitchMapOperator.create(f));

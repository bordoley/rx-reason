let create = observable =>
  observable |> RxObservable.lift(SwitchOperator.create);
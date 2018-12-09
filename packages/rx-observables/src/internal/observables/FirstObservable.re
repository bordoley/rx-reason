let create = observable =>
  observable |> RxObservable.lift(FirstOperator.create());
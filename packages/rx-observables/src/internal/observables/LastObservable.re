let create = observable =>
  observable |> RxObservable.lift(LastOperator.create);
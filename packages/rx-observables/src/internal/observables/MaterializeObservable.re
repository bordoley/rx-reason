let create = observable =>
  observable |> RxObservable.lift(MaterializeOperator.create);
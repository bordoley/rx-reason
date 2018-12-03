let create = observable =>
  observable |> RxObservable.lift(DematerializeOperator.create);
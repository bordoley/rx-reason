let create = observable =>
  observable |> RxObservable.lift(ExhaustOperator.create);
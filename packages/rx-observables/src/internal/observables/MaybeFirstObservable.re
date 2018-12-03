let create = observable =>
  observable |> RxObservable.lift(MaybeFirstOperator.create);
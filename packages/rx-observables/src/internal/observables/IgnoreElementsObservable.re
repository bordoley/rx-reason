let create = observable =>
  observable |> RxObservable.lift(IgnoreElementsOperator.create);
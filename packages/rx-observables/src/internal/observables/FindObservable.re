let create = (predicate, observable) =>
  observable |> RxObservable.lift(FindOperator.create(predicate));
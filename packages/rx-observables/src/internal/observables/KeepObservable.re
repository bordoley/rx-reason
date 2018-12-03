let create = (predicate, observable) =>
  observable |> RxObservable.lift(KeepOperator.create(predicate));
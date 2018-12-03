let create = (predicate, observable) =>
  observable |> RxObservable.lift(EveryOperator.create(predicate));
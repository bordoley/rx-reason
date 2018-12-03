let create = (predicate, observable) =>
  observable |> RxObservable.lift(SomeOperator.create(predicate));
let create = (value, observable) =>
  observable |> RxObservable.lift(MapToOperator.create(value));
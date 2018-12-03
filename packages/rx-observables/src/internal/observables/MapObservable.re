let create = (f, observable) =>
  observable |> RxObservable.lift(MapOperator.create(f));
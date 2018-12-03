let create = (default, observable) =>
  observable |> RxObservable.lift(DefaultIfEmptyOperator.create(default));
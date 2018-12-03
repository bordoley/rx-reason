let create = observable =>
  observable |> RxObservable.lift(IsEmptyOperator.create);
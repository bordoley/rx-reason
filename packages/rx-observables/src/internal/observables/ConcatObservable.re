let create = observable =>
  observable |> RxObservable.lift(ConcatOperator.create);
let create = (f, observable) =>
  observable |> RxObservable.lift(ConcatMapOperator.create(f));
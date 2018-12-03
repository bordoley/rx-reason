let create = (f, observable) =>
  observable |> RxObservable.lift(ExhaustMapOperator.create(f));
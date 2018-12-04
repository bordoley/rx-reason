let create = (skipCount, observable) =>
  observable |> RxObservable.lift(SkipOperator.create(skipCount));
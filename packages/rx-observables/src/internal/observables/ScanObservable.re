let create = (reducer, initialValue, observable) =>
  observable |> RxObservable.lift(ScanOperator.create(reducer, initialValue));
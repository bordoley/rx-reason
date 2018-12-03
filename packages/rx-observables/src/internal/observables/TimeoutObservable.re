let create = (~scheduler, due, observable) =>
  observable |> RxObservable.lift(TimeoutOperator.create(~scheduler, due));
let create = (~scheduler, delay, observable) =>
  observable |> RxObservable.lift(DelayOperator.create(~scheduler, delay));
let create = (takeCount, observable) =>
  observable |> RxObservable.lift(TakeOperator.create(takeCount));
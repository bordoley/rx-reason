let create = (notifier, observable) =>
  observable |> RxObservable.lift(TakeUntilOperator.create(notifier));
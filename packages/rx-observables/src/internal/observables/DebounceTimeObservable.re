let create = (~scheduler, dueTime, observable) =>
  observable
  |> RxObservable.lift(DebounceTimeOperator.create(~scheduler, dueTime));
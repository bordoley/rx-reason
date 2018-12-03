let create = (~scheduler, dueTime, observable) =>
  observable
  |> RxObservable.lift(DebounceOperator.create(~scheduler, dueTime));
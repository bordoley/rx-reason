let create = observables =>
  observables
  |> OfListObservable.create
  |> RxObservable.lift(MergeOperator.create);
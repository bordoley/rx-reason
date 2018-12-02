let mergeList = observables =>
  observables
  |> OfObservable.ofList
  |> RxObservable.lift(MergeOperator.operator);
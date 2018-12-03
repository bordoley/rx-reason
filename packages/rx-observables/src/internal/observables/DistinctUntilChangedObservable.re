let create = (~equals=?, observable) =>
  observable
  |> RxObservable.lift(DistinctUntilChangedOperator.create(~equals?));
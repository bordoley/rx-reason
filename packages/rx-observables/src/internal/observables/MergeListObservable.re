let create = (~scheduler=?, ~maxConcurrency=?, observables) =>
  observables
  |> OfListObservable.create(~scheduler?)
  |> RxObservable.lift(MergeOperator.create(~maxConcurrency?));
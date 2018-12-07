let create = (~scheduler=?, observables) =>
  observables
  |> OfListObservable.create(~scheduler?)
  |> RxObservable.lift(MergeOperator.create);
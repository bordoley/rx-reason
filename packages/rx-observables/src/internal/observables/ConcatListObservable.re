let create = (~scheduler=?, observables) =>
  observables |> MergeListObservable.create(~scheduler?, ~maxConcurrency=1);
let create = (~maxBufferSize=?, ~maxConcurrency=?, f, observable) =>
  observable
  |> RxObservable.lift(
       MergeMapOperator.create(~maxBufferSize?, ~maxConcurrency?, f),
     );
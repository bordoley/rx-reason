let create = (~maxBufferSize=?, ~maxConcurrency=?, observable) =>
  observable
  |> RxObservable.lift(
       MergeOperator.create(~maxBufferSize?, ~maxConcurrency?),
     );

let create = (~maxBufferSize=?, ~maxConcurrency=?, f) => {
  let mapOperator = MapOperator.create(f);
  let mergeOperator = MergeOperator.create(~maxBufferSize?, ~maxConcurrency?);

  s => mapOperator @@ mergeOperator @@ s;
};
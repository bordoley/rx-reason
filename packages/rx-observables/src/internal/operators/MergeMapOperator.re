let create = (~maxBufferSize=?, ~maxConcurrency=?, f) => {
  let mapOperator = MapOperator.create(f);
  let mergeOperator = MergeOperator.create(~maxBufferSize?, ~maxConcurrency?);

  s => mapOperator @@ mergeOperator @@ s;
};

let create1 = (~maxBufferSize=?, ~maxConcurrency=?, f, ctx0) => {
  let mapOperator = MapOperator.create1(f, ctx0);
  let mergeOperator = MergeOperator.create(~maxBufferSize?, ~maxConcurrency?);

  s => mapOperator @@ mergeOperator @@ s;
};

let create2 = (~maxBufferSize=?, ~maxConcurrency=?, f, ctx0, ctx1) => {
  let mapOperator = MapOperator.create2(f, ctx0, ctx1);
  let mergeOperator = MergeOperator.create(~maxBufferSize?, ~maxConcurrency?);

  s => mapOperator @@ mergeOperator @@ s;
};

let create3 = (~maxBufferSize=?, ~maxConcurrency=?, f, ctx0, ctx1, ctx2) => {
  let mapOperator = MapOperator.create3(f, ctx0, ctx1, ctx2);
  let mergeOperator = MergeOperator.create(~maxBufferSize?, ~maxConcurrency?);

  s => mapOperator @@ mergeOperator @@ s;
};
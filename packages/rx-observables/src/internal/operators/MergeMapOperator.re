let operator = (~maxBufferSize=?, ~maxConcurrency=?, f, s) =>
  MapOperator.operator(f) @@
  MergeOperator.operator(~maxBufferSize?, ~maxConcurrency?) @@
  s;
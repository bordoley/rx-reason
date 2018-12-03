let create = (~maxBufferSize=?, ~maxConcurrency=?, f, s) =>
  MapOperator.create(f) @@
  MergeOperator.create(~maxBufferSize?, ~maxConcurrency?) @@
  s;
let operator = (predicate, subscriber) =>
  MapOperator.operator(predicate) @@
  KeepOperator.operator(Functions.identity) @@
  IsEmptyOperator.operator @@
  subscriber;
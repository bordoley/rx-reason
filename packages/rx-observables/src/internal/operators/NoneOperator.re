let operator = (predicate, subscriber) =>
  MapOperator.operator(predicate) @@
  KeepOperator.operator(RxFunctions.identity) @@
  IsEmptyOperator.operator @@
  subscriber;
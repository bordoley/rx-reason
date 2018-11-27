let operator = subscriber =>
  LastOperator.operator @@
  MapOperator.operator(RxFunctions.some) @@
  DefaultIfEmptyOperator.operator(None) @@
  subscriber;
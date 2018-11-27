let operator = subscriber =>
  FirstOperator.operator @@
  MapOperator.operator(RxFunctions.some) @@
  DefaultIfEmptyOperator.operator(None) @@
  subscriber;
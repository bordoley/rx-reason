let operator = subscriber =>
  LastOperator.operator @@
  MapOperator.operator(Functions.some) @@
  DefaultIfEmptyOperator.operator(None) @@
  subscriber;
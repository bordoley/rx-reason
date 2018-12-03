let create = subscriber =>
  LastOperator.create @@
  MapOperator.create(RxFunctions.some) @@
  DefaultIfEmptyOperator.create(None) @@
  subscriber;
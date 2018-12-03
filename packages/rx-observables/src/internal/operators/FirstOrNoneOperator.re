let create = subscriber =>
  FirstOperator.create @@
  MapOperator.create(RxFunctions.some) @@
  DefaultIfEmptyOperator.create(None) @@
  subscriber;
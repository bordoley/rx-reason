let create = (predicate, subscriber) =>
  MapOperator.create(predicate) @@
  KeepOperator.create(RxFunctions.identity) @@
  IsEmptyOperator.create @@
  subscriber;
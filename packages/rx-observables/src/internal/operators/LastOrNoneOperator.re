let create = subscriber =>
  LastOperator.create() @@
  MapOperator.create((. a) => Some(a)) @@
  DefaultIfEmptyOperator.create(None) @@
  subscriber;
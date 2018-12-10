let create = subscriber =>
  FirstOperator.create() @@
  MapOperator.create((. a) => Some(a)) @@
  DefaultIfEmptyOperator.create(None) @@
  subscriber;
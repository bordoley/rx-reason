let create = subscriber =>
  LastOperator.create() @@ MaybeOperator.create() @@ subscriber;
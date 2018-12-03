let create = subscriber =>
  FirstOperator.create @@ MaybeOperator.create @@ subscriber;
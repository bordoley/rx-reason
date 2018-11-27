let operator = subscriber =>
  LastOperator.operator @@ MaybeOperator.operator @@ subscriber;
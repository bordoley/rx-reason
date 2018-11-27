let operator = subscriber =>
  FirstOperator.operator @@ MaybeOperator.operator @@ subscriber;
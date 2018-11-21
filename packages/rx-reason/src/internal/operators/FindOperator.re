let operator = (predicate, subscriber) =>
  KeepOperator.operator(predicate) @@ MaybeFirstOperator.operator @@ subscriber;
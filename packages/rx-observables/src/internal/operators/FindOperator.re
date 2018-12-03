let create = (predicate, subscriber) =>
  KeepOperator.create(predicate) @@ MaybeFirstOperator.create @@ subscriber;
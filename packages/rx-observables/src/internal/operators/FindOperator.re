let create = predicate => {
  let keepOperator = KeepOperator.create(predicate);
  subscriber => keepOperator @@ MaybeFirstOperator.create @@ subscriber;
};
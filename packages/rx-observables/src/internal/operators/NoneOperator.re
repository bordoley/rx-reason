let keepIfTrueOperator = KeepOperator.create((. a) => a);

let create = predicate => {
  let mapOperator = MapOperator.create(predicate);

  subscriber =>
    mapOperator @@ keepIfTrueOperator @@ IsEmptyOperator.create() @@ subscriber;
};
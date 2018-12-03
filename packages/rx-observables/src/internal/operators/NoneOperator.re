let keepIfTrueOperator = KeepOperator.create(RxFunctions.identity);

let create = predicate => {
  let mapOperator = MapOperator.create(predicate);

  subscriber =>
    mapOperator @@ keepIfTrueOperator @@ IsEmptyOperator.create @@ subscriber;
};
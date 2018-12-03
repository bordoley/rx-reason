let create = f => {
  let mapOperator = MapOperator.create(f);
  s => mapOperator @@ ExhaustOperator.create @@ s;
};
let create = f => {
  let mapOperator = MapOperator.create(f);
  s => mapOperator @@ SwitchOperator.create @@ s;
};
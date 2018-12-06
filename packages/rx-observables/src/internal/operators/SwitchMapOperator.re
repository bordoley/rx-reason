let create = f => {
  let mapOperator = MapOperator.create(f);
  s => mapOperator @@ SwitchOperator.create @@ s;
};

let create1 = (f, ctx0) => {
  let mapOperator = MapOperator.create1(f, ctx0);
  s => mapOperator @@ SwitchOperator.create @@ s;
};
let create = f => {
  let mapOperator = MapOperator.create(f);
  s => mapOperator @@ ExhaustOperator.create @@ s;
};

let create1 = (f, ctx0) => {
  let mapOperator = MapOperator.create1(f, ctx0);
  s => mapOperator @@ ExhaustOperator.create @@ s;
};

let create2 = (f, ctx0, ctx1) => {
  let mapOperator = MapOperator.create2(f, ctx0, ctx1);
  s => mapOperator @@ ExhaustOperator.create @@ s;
};

let create3 = (f, ctx0, ctx1, ctx2) => {
  let mapOperator = MapOperator.create3(f, ctx0, ctx1, ctx2);
  s => mapOperator @@ ExhaustOperator.create @@ s;
};
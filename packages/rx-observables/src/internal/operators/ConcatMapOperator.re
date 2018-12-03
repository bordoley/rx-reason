let create = f => {
  let mapOperator = MapOperator.create(f);
  s => mapOperator @@ ConcatOperator.create @@ s;
};
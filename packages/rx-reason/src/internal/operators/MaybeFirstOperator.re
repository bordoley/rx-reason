let operator = subscriber =>
  FirstOperator.operator @@ MaybeOperator.operator @@ subscriber;

let lift = observable => observable |> ObservableSource.lift(operator);
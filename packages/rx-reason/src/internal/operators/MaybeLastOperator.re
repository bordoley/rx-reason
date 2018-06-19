let operator = subscriber => LastOperator.operator @@ MaybeOperator.operator @@ subscriber;

let lift = observable => observable |> ObservableSource.lift(operator);
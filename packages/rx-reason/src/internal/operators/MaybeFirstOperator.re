let operator = observer => FirstOperator.operator @@ MaybeOperator.operator @@ observer;

let lift = observable => observable |> ObservableSource.lift(operator);
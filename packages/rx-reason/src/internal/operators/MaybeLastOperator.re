let operator = observer => LastOperator.operator @@ MaybeOperator.operator @@ observer;

let lift = observable => observable |> ObservableSource.lift(operator);
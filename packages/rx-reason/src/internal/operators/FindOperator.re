let operator = (predicate, observer) =>
  KeepOperator.operator(predicate) @@ MaybeFirstOperator.operator @@ observer;

let lift = (predicate, observable) =>
   observable |> ObservableSource.lift(operator(predicate));
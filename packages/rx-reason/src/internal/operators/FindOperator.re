let operator = (predicate, subscriber) =>
  KeepOperator.operator(predicate) @@ MaybeFirstOperator.operator @@ subscriber;

let lift = (predicate, observable) =>
   observable |> ObservableSource.lift(operator(predicate));
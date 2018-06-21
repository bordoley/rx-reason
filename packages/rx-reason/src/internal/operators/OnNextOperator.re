let operator = onNext => ObserveOperator.operator(~onNext, ~onComplete=Functions.alwaysUnit1);
let lift = (onComplete, observable) =>
  observable |> ObservableSource.lift(operator(onComplete));
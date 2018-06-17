let operator = onNext => ObserveOperator.operator(~onNext, ~onComplete=Functions.alwaysUnit);
let lift = (onComplete, observable) =>
  observable |> ObservableSource.lift(operator(onComplete));
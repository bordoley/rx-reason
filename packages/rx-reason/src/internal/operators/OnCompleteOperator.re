let operator = onComplete =>
  ObserveOperator.operator(~onNext=Functions.alwaysUnit, ~onComplete);

let lift = (onComplete, observable) =>
  observable |> ObservableSource.lift(operator(onComplete));
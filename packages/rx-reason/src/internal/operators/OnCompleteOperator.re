let operator = onComplete =>
  ObserveOperator.operator(~onNext=Functions.alwaysUnit1, ~onComplete);

let lift = (onComplete, observable) =>
  observable |> ObservableSource.lift(operator(onComplete));
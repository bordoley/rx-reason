let operator = onComplete =>
  ObserveOperator.operator(~onNext=Functions.alwaysUnit1, ~onComplete);
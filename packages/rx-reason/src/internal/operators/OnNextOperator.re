let operator = onNext =>
  ObserveOperator.operator(~onNext, ~onComplete=Functions.alwaysUnit1);
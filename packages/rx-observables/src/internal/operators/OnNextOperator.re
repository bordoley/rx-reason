let operator = onNext =>
  ObserveOperator.operator(~onNext, ~onComplete=RxFunctions.alwaysUnit1);
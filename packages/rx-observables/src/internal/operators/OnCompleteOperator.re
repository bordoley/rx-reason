let operator = onComplete =>
  ObserveOperator.operator(~onNext=RxFunctions.alwaysUnit1, ~onComplete);
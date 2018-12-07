let create = (onComplete, subscriber) =>
  ObserveOperator.create(
    ~onNext=RxFunctions.alwaysUnit1,
    ~onComplete,
    subscriber,
  );

let create1 = (onComplete, ctx0, subscriber) =>
  ObserveOperator.create1(
    ~onNext=RxFunctions.alwaysUnit2,
    ~onComplete,
    ctx0,
    subscriber,
  );

let create2 = (onComplete, ctx0, ctx1, subscriber) =>
  ObserveOperator.create2(
    ~onNext=RxFunctions.alwaysUnit3,
    ~onComplete,
    ctx0,
    ctx1,
    subscriber,
  );
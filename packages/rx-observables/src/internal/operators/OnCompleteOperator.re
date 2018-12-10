let create = (onComplete,) =>
  ObserveOperator.create(
    ~onNext=(. _) => (),
    ~onComplete,
  );

let create1 = (onComplete, ctx0) =>
  ObserveOperator.create1(
    ~onNext=RxFunctions.alwaysUnit2,
    ~onComplete,
    ctx0,
  );

let create2 = (onComplete, ctx0, ctx1) =>
  ObserveOperator.create2(
    ~onNext=RxFunctions.alwaysUnit3,
    ~onComplete,
    ctx0,
    ctx1,
  );

let create3 = (onComplete, ctx0, ctx1, ctx2) =>
  ObserveOperator.create3(
    ~onNext=RxFunctions.alwaysUnit4,
    ~onComplete,
    ctx0,
    ctx1,
    ctx2,
  );
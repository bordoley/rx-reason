let create = onExn => {
  let onComplete =
    fun
    | None => ()
    | Some(exn) => onExn(exn);

  subscriber =>
    ObserveOperator.create(
      ~onNext=RxFunctions.alwaysUnit1,
      ~onComplete,
      subscriber,
    );
};

let create1 = onExn => {
  let onComplete = ctx0 =>
    fun
    | None => ()
    | Some(exn) => onExn(ctx0, exn);

  (ctx0, subscriber) =>
    ObserveOperator.create1(
      ~onNext=RxFunctions.alwaysUnit2,
      ~onComplete,
      ctx0,
      subscriber,
    );
};

let create2 = onExn => {
  let onComplete = (ctx0, ctx1) =>
    fun
    | None => ()
    | Some(exn) => onExn(ctx0, ctx1, exn);

  (ctx0, ctx1, subscriber) =>
    ObserveOperator.create2(
      ~onNext=RxFunctions.alwaysUnit3,
      ~onComplete,
      ctx0,
      ctx1,
      subscriber,
    );
};

let create3 = onExn => {
  let onComplete = (ctx0, ctx1, ctx2) =>
    fun
    | None => ()
    | Some(exn) => onExn(ctx0, ctx1, ctx2, exn);

  (ctx0, ctx1, ctx2, subscriber) =>
    ObserveOperator.create3(
      ~onNext=RxFunctions.alwaysUnit4,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      subscriber,
    );
};
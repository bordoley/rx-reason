let create = (onExn, observable) =>
  observable |> RxObservable.lift(OnExnOperator.create(onExn));

let create1 = (onExn, ctx0, observable) =>
  observable |> RxObservable.lift(OnExnOperator.create1(onExn, ctx0));

let create2 = (onExn, ctx0, ctx1, observable) =>
  observable |> RxObservable.lift(OnExnOperator.create2(onExn, ctx0, ctx1));

let create3 = (onExn, ctx0, ctx1, ctx2, observable) =>
  observable |> RxObservable.lift(OnExnOperator.create3(onExn, ctx0, ctx1, ctx2));
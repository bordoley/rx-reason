let create = (f, observable) =>
  observable |> RxObservable.lift(OnConnectOperator.create(f));

let create1 = (f, ctx0, observable) =>
  observable |> RxObservable.lift(OnConnectOperator.create1(f, ctx0));

let create2 = (f, ctx0, ctx1, observable) =>
  observable |> RxObservable.lift(OnConnectOperator.create2(f, ctx0, ctx1));

let create3 = (f, ctx0, ctx1, ctx2, observable) =>
  observable |> RxObservable.lift(OnConnectOperator.create3(f, ctx0, ctx1, ctx2));
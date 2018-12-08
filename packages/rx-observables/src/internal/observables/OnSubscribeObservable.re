let create = (f, observable) =>
  observable |> RxObservable.lift(OnSubscribeOperator.create(f));

let create1 = (f, ctx0, observable) =>
  observable |> RxObservable.lift(OnSubscribeOperator.create1(f, ctx0));

let create2 = (f, ctx0, ctx1, observable) =>
  observable |> RxObservable.lift(OnSubscribeOperator.create2(f, ctx0, ctx1));
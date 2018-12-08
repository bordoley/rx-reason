let create = (f, subscriber) =>
  subscriber |> RxSubscriber.addDisposable(f());

let create1 = (f, ctx0, subscriber) =>
  subscriber |> RxSubscriber.addDisposable(f(ctx0));

let create2 = (f, ctx0, ctx1, subscriber) =>
  subscriber |> RxSubscriber.addDisposable(f(ctx0, ctx1));
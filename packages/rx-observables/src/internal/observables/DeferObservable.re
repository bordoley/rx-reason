let defer = {
  let source = (f, subscriber) =>
    f() |> RxObservable.subscribeWith(subscriber);
  f => RxObservable.create1(source, f);
};
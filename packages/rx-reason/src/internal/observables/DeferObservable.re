let defer = {
  let source = (f, subscriber) =>
    f() |> Observable.subscribeWith(subscriber);
  f => Observable.create1(source, f);
};
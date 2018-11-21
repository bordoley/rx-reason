let defer = {
  let source = (f, subscriber) =>
    f() |> Observable.subscribeSubscriber(subscriber);
  f => Observable.create1(source, f);
};
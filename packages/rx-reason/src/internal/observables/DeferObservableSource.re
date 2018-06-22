let defer = {
  let source = (f, subscriber) =>
    f() |> ObservableSource.subscribeSubscriber(subscriber);
  f => ObservableSource.create1(source, f);
};
let operator = (f, subscriber) =>
  subscriber |> RxSubscriber.addDisposable(f());
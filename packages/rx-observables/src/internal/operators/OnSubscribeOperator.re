let create = (f, subscriber) =>
  subscriber |> RxSubscriber.addDisposable(f());
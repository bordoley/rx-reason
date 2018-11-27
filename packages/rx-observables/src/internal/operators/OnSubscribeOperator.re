let operator = (f, subscriber) =>
  subscriber |> RxSubscriber.addTeardown1(RxDisposable.dispose, f());
let operator = (f, subscriber) =>
  subscriber |> Subscriber.addTeardown1(Disposable.dispose, f());
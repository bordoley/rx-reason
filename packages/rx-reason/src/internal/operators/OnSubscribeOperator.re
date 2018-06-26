let operator = (f, subscriber) =>
  subscriber |> Subscriber.addTeardown1(Disposable.dispose, f());

let lift = (f, observable) =>
  observable |> ObservableSource.lift(operator(f));
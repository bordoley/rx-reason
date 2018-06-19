let operator = (f, subscriber) => subscriber |> Subscriber.addTeardown(f());

let lift = (f, observable) =>
  observable |> ObservableSource.lift(operator(f));
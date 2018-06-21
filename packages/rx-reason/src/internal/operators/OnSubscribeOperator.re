let operator = (f, subscriber) => subscriber |> Subscriber.addDisposable(f());

let lift = (f, observable) =>
  observable |> ObservableSource.lift(operator(f));
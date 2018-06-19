let operator = (f, observer) => observer |> Observer.addTeardown(f());

let lift = (f, observable) =>
  observable |> ObservableSource.lift(operator(f));
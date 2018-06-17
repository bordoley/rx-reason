let operator = value => MapOperator.operator(_ => value);

let lift = (value, observable) =>
  observable |> ObservableSource.lift(operator(value));
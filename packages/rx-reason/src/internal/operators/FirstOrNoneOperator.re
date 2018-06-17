let operator = observer =>
  FirstOperator.operator @@
  MapOperator.operator(a => Some(a)) @@
  DefaultIfEmptyOperator.operator(None) @@
  observer;

let lift = observable =>
  observable |> ObservableSource.lift(operator);
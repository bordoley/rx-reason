let operator = subscriber =>
  FirstOperator.operator @@
  MapOperator.operator(a => Some(a)) @@
  DefaultIfEmptyOperator.operator(None) @@
  subscriber;

let lift = observable =>
  observable |> ObservableSource.lift(operator);
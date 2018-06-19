let operator = subscriber =>
  LastOperator.operator @@
  MapOperator.operator(a => Some(a)) @@
  DefaultIfEmptyOperator.operator(None) @@
  subscriber;

let lift = observable =>
observable |> ObservableSource.lift(operator);
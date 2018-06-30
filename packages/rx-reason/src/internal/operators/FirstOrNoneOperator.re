let operator = subscriber =>
  FirstOperator.operator @@
  MapOperator.operator(Functions.some) @@
  DefaultIfEmptyOperator.operator(None) @@
  subscriber;

let lift = observable =>
  observable |> ObservableSource.lift(operator);
let operator = (predicate, subscriber) =>
  MapOperator.operator(predicate) @@
  KeepOperator.operator(Functions.identity) @@
  IsEmptyOperator.operator @@
  subscriber;

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));
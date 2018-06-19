let operator = (predicate, subscriber) =>
  MapOperator.operator(predicate) @@
  KeepOperator.operator(x => x) @@
  IsEmptyOperator.operator @@
  subscriber;

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));
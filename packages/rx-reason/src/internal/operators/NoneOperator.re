let operator = (predicate, observer) =>
  MapOperator.operator(predicate) @@
  KeepOperator.operator(x => x) @@
  IsEmptyOperator.operator @@
  observer;

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));
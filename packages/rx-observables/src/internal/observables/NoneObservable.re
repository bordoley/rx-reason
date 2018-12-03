let create = (predicate, observable) =>
  observable |> RxObservable.lift(NoneOperator.create(predicate));

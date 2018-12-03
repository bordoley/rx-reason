let create = observable =>
  observable |> RxObservable.lift(LastOrNoneOperator.create);

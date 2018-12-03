let create = observable =>
  observable |> RxObservable.lift(FirstOrNoneOperator.create);
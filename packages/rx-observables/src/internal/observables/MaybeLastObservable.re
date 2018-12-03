let create = observable =>
  observable |> RxObservable.lift(MaybeLastOperator.create)
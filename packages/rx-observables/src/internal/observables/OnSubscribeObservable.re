let create = (f, observable) =>
  observable |> RxObservable.lift(OnSubscribeOperator.create(f));
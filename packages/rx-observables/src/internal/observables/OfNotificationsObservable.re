let create = (~scheduler=?, notifications) =>
  OfListObservable.create(~scheduler?, notifications)
  |> RxObservable.lift(DematerializeOperator.create());
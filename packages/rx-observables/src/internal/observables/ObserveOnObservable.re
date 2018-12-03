let create = (scheduler, observable) =>
  observable |> RxObservable.lift(ObserveOnOperator.create(scheduler));
let create = (~selector, other, source) =>
  source |> RxObservable.lift(WithLatestFromOperator.create(~selector, other));
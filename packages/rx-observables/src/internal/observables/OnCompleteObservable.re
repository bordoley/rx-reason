let create = (onComplete, observable) =>
  observable
  |> ObserveObservable.create(~onNext=RxFunctions.alwaysUnit1, ~onComplete);
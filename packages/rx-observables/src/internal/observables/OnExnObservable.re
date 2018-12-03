let create = (onExn, observable) =>
  observable
  |> ObserveObservable.create(
       ~onNext=RxFunctions.alwaysUnit1,
       ~onComplete=
         fun
         | None => ()
         | Some(exn) => onExn(exn),
     );
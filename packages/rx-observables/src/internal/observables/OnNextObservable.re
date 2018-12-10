let create = (onNext, observable) =>
  observable
  |> ObserveObservable.create(~onNext, ~onComplete=(._) => ());

let create1 = (onNext, ctx0, observable) =>
  observable
  |> ObserveObservable.create1(
       ~onNext,
       ~onComplete=RxFunctions.alwaysUnit2,
       ctx0,
     );

let create2 = (onNext, ctx0, ctx1, observable) =>
  observable
  |> ObserveObservable.create2(
       ~onNext,
       ~onComplete=RxFunctions.alwaysUnit3,
       ctx0,
       ctx1,
     );

let create3 = (onNext, ctx0, ctx1, ctx2, observable) =>
  observable
  |> ObserveObservable.create3(
       ~onNext,
       ~onComplete=RxFunctions.alwaysUnit4,
       ctx0,
       ctx1,
       ctx2,
     );
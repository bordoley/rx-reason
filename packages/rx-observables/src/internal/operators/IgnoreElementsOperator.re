let operator = subscriber =>
  subscriber
  |> RxSubscriber.decorate(
       ~onNext=RxFunctions.alwaysUnit2,
       ~onComplete=RxSubscriber.forwardOnComplete,
     );
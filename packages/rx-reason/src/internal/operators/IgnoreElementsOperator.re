let operator = subscriber =>
  subscriber
  |> Subscriber.decorate(
       ~onNext=Functions.alwaysUnit2,
       ~onComplete=Subscriber.forwardOnComplete,
     );
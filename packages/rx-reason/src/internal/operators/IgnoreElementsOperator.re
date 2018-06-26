let operator = subscriber =>
  subscriber
  |> Subscriber.decorate(
       ~onNext=Functions.alwaysUnit2,
       ~onComplete=Subscriber.forwardOnComplete,
     );

let lift = observable => observable |> ObservableSource.lift(operator);
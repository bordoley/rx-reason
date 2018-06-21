let operator = subscriber =>
  subscriber
  |> Subscriber.delegate(
       ~onNext=Functions.alwaysUnit3,
       ~onComplete=Subscriber.forwardOnComplete,
       (),
     );

let lift = observable => observable |> ObservableSource.lift(operator);
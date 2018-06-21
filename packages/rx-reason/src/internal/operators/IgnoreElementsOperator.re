let operator = subscriber =>
  subscriber
  |> Subscriber.delegate(
       ~onNext=Functions.alwaysUnit2,
       ~onComplete=Subscriber.delegateOnComplete,
     );

let lift = observable => observable |> ObservableSource.lift(operator);
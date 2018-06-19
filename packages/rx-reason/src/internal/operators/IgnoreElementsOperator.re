let operator = subscriber =>
  Subscriber.delegate(
    ~onNext=Functions.alwaysUnit,
    ~onComplete=Subscriber.forwardOnComplete(subscriber),
    subscriber,
  );

let lift = observable => observable |> ObservableSource.lift(operator);
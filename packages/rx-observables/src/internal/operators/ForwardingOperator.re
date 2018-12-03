let operator = subscriber =>
  ObserveOperator.operator1(
    ~onNext=SubscriberForward.onNext,
    ~onComplete=SubscriberForward.onComplete,
    subscriber,
  );
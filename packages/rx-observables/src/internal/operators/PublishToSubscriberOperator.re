let create = subscriber =>
  ObserveOperator.create1(
    ~onNext=SubscriberForward.onNext,
    ~onComplete=SubscriberForward.onComplete,
    subscriber,
  );
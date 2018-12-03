let create = subscriber =>
  RxSubscriber.decorate(
    ~onNext=RxFunctions.alwaysUnit2,
    ~onComplete=SubscriberForward.onComplete,
    subscriber,
  );
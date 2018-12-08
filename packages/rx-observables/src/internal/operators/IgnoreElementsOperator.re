let create = subscriber =>
  RxSubscriber.decorateOnNext(RxFunctions.alwaysUnit2, subscriber);
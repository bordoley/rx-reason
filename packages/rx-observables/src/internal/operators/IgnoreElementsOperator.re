let create = subscriber =>
  RxSubscriber.decorateOnNext(~onNext=RxFunctions.alwaysUnit2, subscriber);
type context('a) = {
  predicate: 'a => bool,
  mutable self: RxSubscriber.t('a),
};

let onNext = (predicate, subscriber, next) => {
  let shouldKeep = predicate(next);
  if (shouldKeep) {
    subscriber |> RxSubscriber.next(next);
  };
};

let create = predicate =>
  RxSubscriber.decorate1(
    ~onNext,
    ~onComplete=SubscriberForward.onComplete1,
    predicate,
  );
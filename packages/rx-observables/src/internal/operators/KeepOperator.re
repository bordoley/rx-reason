type context('a) = {
  predicate: 'a => bool,
  mutable self: RxSubscriber.t('a),
};

let operator = {
  let onNext = (predicate, subscriber, next) => {
    let shouldKeep = predicate(next);
    if (shouldKeep) {
      subscriber |> RxSubscriber.next(next);
    };
  };

  predicate =>
    RxSubscriber.decorate1(
      ~onNext,
      ~onComplete=RxSubscriber.forwardOnComplete1,
      predicate,
    );
};
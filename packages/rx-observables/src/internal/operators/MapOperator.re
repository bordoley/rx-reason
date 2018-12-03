type context('a, 'b) = {
  mapper: 'a => 'b,
  mutable self: RxSubscriber.t('a),
};

let onNext = (mapper, delegate, next) => {
  let mapped = mapper(next);
  delegate |> RxSubscriber.next(mapped);
};

let create = mapper =>
  RxSubscriber.decorate1(
    ~onNext,
    ~onComplete=SubscriberForward.onComplete1,
    mapper,
  );
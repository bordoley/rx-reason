let onNext = (value, delegate, _) => delegate |> RxSubscriber.next(value);

let create = (value, subscriber) =>
  RxSubscriber.decorate1(
    ~onNext,
    ~onComplete=SubscriberForward.onComplete1,
    value,
    subscriber,
  );
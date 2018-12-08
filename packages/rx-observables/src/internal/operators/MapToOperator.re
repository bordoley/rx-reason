let onNext = (value, delegate, _) => delegate |> RxSubscriber.next(value);

let create = (value, subscriber) =>
  RxSubscriber.decorateOnNext1(onNext, value, subscriber);
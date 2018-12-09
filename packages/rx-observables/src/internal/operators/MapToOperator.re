let create = value => {
  let onNext =
    (. value, delegate, _) => delegate |> RxSubscriber.next(value);
  RxSubscriber.decorateOnNext1(onNext, value);
};
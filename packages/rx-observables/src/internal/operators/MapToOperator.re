let operator = {
  let onNext = (value, delegate, _) => delegate |> RxSubscriber.next(value);

  (value, subscriber) =>
    subscriber
    |> RxSubscriber.decorate1(
         ~onNext,
         ~onComplete=SubscriberForward.onComplete1,
         value,
       );
};
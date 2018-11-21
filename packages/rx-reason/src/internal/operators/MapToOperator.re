let operator = {
  let onNext = (value, delegate, _) => delegate |> Subscriber.next(value);

  (value, subscriber) =>
    subscriber
    |> Subscriber.decorate1(
         ~onNext,
         ~onComplete=Subscriber.forwardOnComplete1,
         value,
       );
};
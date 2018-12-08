let onNext = (remainder, subscriber, next) => { 
  decr(remainder);
  subscriber |> RxSubscriber.next(next);

  if (remainder^ === 0) {
    subscriber |> RxSubscriber.complete;
  }
};

let create = takeCount => {
  RxPreconditions.checkArgument(
    takeCount > 0,
    "TakeOperator: takeCount must be greater than 0",
  );

  subscriber => {
    let remainder = ref(takeCount);
    subscriber |> RxSubscriber.decorateOnNext1(onNext, remainder);
  };
};
let create = (subscriber, observable) => observable |> RxObservable.lift(
  PublishToSubscriberOperator.create(subscriber)
);
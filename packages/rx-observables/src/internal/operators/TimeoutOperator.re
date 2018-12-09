let subscribeToTimeout = (timeoutObservable, timeoutSubscription) => {
  timeoutSubscription
  |> RxSerialDisposable.getInnerDisposable
  |> RxDisposable.dispose;
  timeoutSubscription
  |> RxSerialDisposable.setInnerDisposable(
       timeoutObservable |> RxObservable.connect,
     );
};

let create = (~scheduler, due) => {
  RxPreconditions.checkArgument(
    due > 0.0,
    "TimeoutOperator: due time must be greater than 0.0 milliseconds",
  );

  let onNext =
    (. timeoutObservable, timeoutSubscription, delegate, next) => {
      delegate |> RxSubscriber.next(next);
      subscribeToTimeout(timeoutObservable, timeoutSubscription);
    };

  let onComplete =
    (. _, timeoutSubscription, delegate, exn) => {
      timeoutSubscription |> RxSerialDisposable.dispose;
      delegate |> RxSubscriber.complete(~exn?);
    };

  subscriber => {
    let timeoutObservable =
      RaiseObservable.create(RxTimeoutException.Exn)
      |> DelayObservable.create(~scheduler, due)
      |> IgnoreElementsObservable.create
      |> PublishToSubscriberObservable.create(subscriber);

    let timeoutSubscription = RxSerialDisposable.create();
    let timeoutDisposable =
      timeoutSubscription |> RxSerialDisposable.asDisposable;

    let self =
      subscriber
      |> RxSubscriber.decorate2(
           ~onNext,
           ~onComplete,
           timeoutObservable,
           timeoutSubscription,
         )
      |> RxSubscriber.addDisposable(timeoutDisposable);

    subscribeToTimeout(timeoutObservable, timeoutSubscription);
    self;
  };
};
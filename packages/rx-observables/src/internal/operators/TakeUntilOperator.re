module NotifierObservable = {
  let onNext = (_, seenValue, _) => seenValue := true;

  let onComplete = (takeUntilSubscriber, _, exn) =>
    switch (exn) {
    | Some(_) => takeUntilSubscriber |> RxSubscriber.complete(~exn?)
    | None => ()
    };

  let create = (takeUntilSubscriber, seenValue) =>
    ObserveObservable.create2(
      ~onNext,
      ~onComplete,
      takeUntilSubscriber,
      seenValue,
    );
};

let onNext = (seenValue, delegate, next) =>
  if (! seenValue^) {
    delegate |> RxSubscriber.next(next);
  };

let create = (notifier, subscriber) => {
  let seenValue = ref(false);

  let takeUntilSubscriber =
    RxSubscriber.decorate1(
      ~onNext,
      ~onComplete=SubscriberForward.onComplete1,
      seenValue,
      subscriber,
    );

  let notifierSubscription =
    notifier
    |> NotifierObservable.create(takeUntilSubscriber, seenValue)
    |> RxObservable.subscribe;

  takeUntilSubscriber |> RxSubscriber.addDisposable(notifierSubscription);
};
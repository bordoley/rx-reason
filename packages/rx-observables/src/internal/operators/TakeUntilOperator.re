module NotifierObservable = {
  let onNext = (takeUntilSubscriber, _) =>
    takeUntilSubscriber |> RxSubscriber.complete;

  let onComplete = (takeUntilSubscriber, exn) =>
    switch (exn) {
    | Some(_) => takeUntilSubscriber |> RxSubscriber.complete(~exn?)
    | None => ()
    };

  let create = takeUntilSubscriber =>
    ObserveObservable.create1(~onNext, ~onComplete, takeUntilSubscriber);
};

let create = (notifier, subscriber) => {
  let notifierSubscription =
    notifier
    |> NotifierObservable.create(subscriber)
    |> RxObservable.subscribe;

  subscriber |> RxSubscriber.addDisposable(notifierSubscription);
};
let operator = {
  let rec onComplete = (observable, shouldRepeat, subscription, delegate, exn) => {
    let shouldComplete = ! shouldRepeat(exn);

    shouldComplete ?
      delegate |> RxSubscriber.complete(~exn?) :
      setupSubscription(observable, shouldRepeat, subscription, delegate);
  }
  and setupSubscription = (observable, shouldRepeat, subscription, delegate) => {
    let alreadyDisposed = subscription |> RxSerialDisposable.isDisposed;

    if (! alreadyDisposed) {
      subscription
      |> RxSerialDisposable.getInnerDisposable
      |> RxDisposable.dispose;
      let newInnerSubscription =
        observable
        |> RxObservable.observe4(
             ~onNext=SubscriberForward.onNext3,
             ~onComplete,
             observable,
             shouldRepeat,
             subscription,
             delegate,
           )
        |> RxObservable.subscribe;
      subscription
      |> RxSerialDisposable.setInnerDisposable(newInnerSubscription);
    };
  };

  (shouldRepeat, observable, subscriber) => {
    let subscription = RxSerialDisposable.create();

    subscriber
    |> RxSubscriber.decorate3(
         ~onNext=SubscriberForward.onNext3,
         ~onComplete,
         observable,
         shouldRepeat,
         subscription,
       )
    |> RxSubscriber.addTeardown1(RxSerialDisposable.dispose, subscription);
  };
};

let repeat = (predicate, observable) =>
  observable |> RxObservable.lift(operator(predicate, observable));
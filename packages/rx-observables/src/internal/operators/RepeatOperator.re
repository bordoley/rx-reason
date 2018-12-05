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
      |> RxObservable.lift(
           ObserveOperator.create4(
             ~onNext=SubscriberForward.onNext3,
             ~onComplete,
             observable,
             shouldRepeat,
             subscription,
             delegate,
           ),
         )
      |> RxObservable.connect;
    subscription
    |> RxSerialDisposable.setInnerDisposable(newInnerSubscription);
  };
};

let create = (shouldRepeat, observable, subscriber) => {
  let subscription = RxSerialDisposable.create();
  let disposable = subscription |> RxSerialDisposable.asDisposable;
  subscriber
  |> RxSubscriber.decorate3(
       ~onNext=SubscriberForward.onNext3,
       ~onComplete,
       observable,
       shouldRepeat,
       subscription,
     )
  |> RxSubscriber.addDisposable(disposable);
};
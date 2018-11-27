let operator = {
  let rec onComplete = {
    let impl = (observable, shouldRepeat, subscription, delegate, exn) => {
      let shouldComplete =
        try (! shouldRepeat(exn)) {
        | exn =>
          delegate |> RxSubscriber.complete(~exn);
          RxFunctions.returnUnit();
        };

      shouldComplete ?
        delegate |> RxSubscriber.complete(~exn?) :
        setupSubscription(observable, shouldRepeat, subscription, delegate);
    };

    (observable, shouldRepeat, subscription, delegate, exn) =>
      RxFunctions.earlyReturnsUnit5(
        impl,
        observable,
        shouldRepeat,
        subscription,
        delegate,
        exn,
      );
  }
  and setupSubscription = (observable, shouldRepeat, subscription, delegate) => {
    let alreadyDisposed = subscription |> RxSerialDisposable.isDisposed;

    if (! alreadyDisposed) {
      subscription
      |> RxSerialDisposable.getInnerDisposable
      |> RxDisposable.dispose;
      let newInnerSubscription =
        RxSubscriber.create4(
          ~onNext=RxSubscriber.forwardOnNext3,
          ~onComplete,
          observable,
          shouldRepeat,
          subscription,
          delegate,
        );
      observable |> RxObservable.subscribeWith(newInnerSubscription);
      subscription
      |> RxSerialDisposable.setInnerDisposable(
           newInnerSubscription |> RxSubscriber.asDisposable,
         );
    };
  };

  (shouldRepeat, observable, subscriber) => {
    let subscription = RxSerialDisposable.create();

    subscriber
    |> RxSubscriber.decorate3(
         ~onNext=RxSubscriber.forwardOnNext3,
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
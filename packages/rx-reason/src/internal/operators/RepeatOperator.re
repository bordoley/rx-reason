let operator = {
  let rec onComplete = {
    let impl = (observable, shouldRepeat, subscription, delegate, exn) => {
      let shouldComplete =
        try (! shouldRepeat(exn)) {
        | exn =>
          delegate |> Subscriber.complete(~exn);
          Functions.returnUnit();
        };

      shouldComplete ?
        delegate |> Subscriber.complete(~exn?) :
        setupSubscription(observable, shouldRepeat, subscription, delegate);
    };

    (observable, shouldRepeat, subscription, delegate, exn) =>
      Functions.earlyReturnsUnit5(
        impl,
        observable,
        shouldRepeat,
        subscription,
        delegate,
        exn,
      );
  }
  and setupSubscription = (observable, shouldRepeat, subscription, delegate) => {
    let alreadyDisposed = subscription |> SerialDisposable.isDisposed;

    if (! alreadyDisposed) {
      subscription |> SerialDisposable.get |> Disposable.dispose;
      let newInnerSubscription =
        observable
        |> ObservableSource.subscribeWith4(
             ~onNext=Subscriber.delegateOnNext3,
             ~onComplete,
             observable,
             shouldRepeat,
             subscription,
             delegate,
           );
      subscription
      |> SerialDisposable.set(
           newInnerSubscription |> CompositeDisposable.asDisposable,
         );
    };
  };

  (shouldRepeat, observable, subscriber) => {
    let subscription = SerialDisposable.create();

    subscriber
    |> Subscriber.delegate3(
         ~onNext=Subscriber.delegateOnNext3,
         ~onComplete,
         observable,
         shouldRepeat,
         subscription,
       )
    |> Subscriber.addSerialDisposable(subscription);
  };
};

let lift = (shouldRepeat, observable) =>
  observable |> ObservableSource.lift(operator(shouldRepeat, observable));
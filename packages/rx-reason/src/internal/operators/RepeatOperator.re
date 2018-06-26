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
      subscription |> SerialDisposable.getInnerDisposable |> Disposable.dispose;
      let newInnerSubscription =
        observable
        |> ObservableSource.subscribeWith4(
             ~onNext=Subscriber.forwardOnNext3,
             ~onComplete,
             observable,
             shouldRepeat,
             subscription,
             delegate,
           );
      subscription
      |> SerialDisposable.setInnerDisposable(newInnerSubscription);
    };
  };

  (shouldRepeat, observable, subscriber) => {
    let subscription = SerialDisposable.create();

    subscriber
    |> Subscriber.decorate3(
         ~onNext=Subscriber.forwardOnNext3,
         ~onComplete,
         observable,
         shouldRepeat,
         subscription,
       )
    |> Subscriber.addTeardown1(SerialDisposable.dispose, subscription);
  };
};

let lift = (shouldRepeat, observable) =>
  observable |> ObservableSource.lift(operator(shouldRepeat, observable));
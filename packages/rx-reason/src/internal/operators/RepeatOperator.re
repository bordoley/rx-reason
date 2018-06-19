let operator = (shouldRetry, observable, subscriber) => {
  let subscription = SerialDisposable.create();
  let setupSubscription = ref(Functions.alwaysUnit);

  let onComplete =
    Functions.earlyReturnsUnit1(exn => {
      let shouldComplete =
        try (! shouldRetry(exn)) {
        | exn =>
          subscriber |> Subscriber.complete(~exn);
          Functions.returnUnit();
        };

      shouldComplete ?
        subscriber |> Subscriber.complete(~exn?) : setupSubscription^();
    });

  setupSubscription :=
    (
      () => {
        let alreadyDisposed = subscription |> SerialDisposable.isDisposed;

        if (! alreadyDisposed) {
          subscription |> SerialDisposable.get |> Disposable.dispose;
          let newInnerSubscription =
            observable
            |> ObservableSource.subscribeWith(
                 ~onNext=Subscriber.forwardOnNext(subscriber),
                 ~onComplete,
               );
          subscription
          |> SerialDisposable.set(
               newInnerSubscription |> CompositeDisposable.asDisposable,
             );
        };
      }
    );

  subscriber
  |> Subscriber.delegate(
       ~onNext=Subscriber.forwardOnNext(subscriber),
       ~onComplete,
     )
  |> Subscriber.addDisposable(SerialDisposable.asDisposable(subscription));
};

let lift = (shouldRetry, observable) =>
  observable |> ObservableSource.lift(operator(shouldRetry, observable));
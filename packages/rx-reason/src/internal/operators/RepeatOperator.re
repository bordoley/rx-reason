let operator = (shouldRetry, observable, observer) => {
  let subscription = SerialDisposable.create();
  let setupSubscription = ref(Functions.alwaysUnit);

  let onComplete =
    Functions.earlyReturnsUnit1(exn => {
      let shouldComplete =
        try (! shouldRetry(exn)) {
        | exn =>
          observer |> Observer.complete(~exn);
          Functions.returnUnit();
        };

      shouldComplete ?
        observer |> Observer.complete(~exn?) : setupSubscription^();
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
                 ~onNext=Observer.forwardOnNext(observer),
                 ~onComplete,
               );
          subscription
          |> SerialDisposable.set(
               newInnerSubscription |> CompositeDisposable.asDisposable,
             );
        };
      }
    );

  observer
  |> Observer.delegate(
       ~onNext=Observer.forwardOnNext(observer),
       ~onComplete,
     )
  |> Observer.addDisposable(SerialDisposable.asDisposable(subscription));
};

let lift = (shouldRetry, observable) =>
  observable |> ObservableSource.lift(operator(shouldRetry, observable));
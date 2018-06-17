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
          subscription |> SerialDisposable.set(newInnerSubscription);
        };
      }
    );

  Observer.create(
    ~onNext=Observer.forwardOnNext(observer),
    ~onComplete,
    ~onDispose=() => {
      subscription |> SerialDisposable.dispose;
      observer |> Observer.dispose;
    },
  );
};

let lift = (shouldRetry, observable) =>
  observable |> ObservableSource.lift(operator(shouldRetry, observable));
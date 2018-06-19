let operator = scheduler => {
  let timeoutObservable =
    ObservableSource.raise(~scheduler, TimeoutException.Exn);
  observer => {
    let timeoutSubscription = SerialDisposable.create();
    let timeOutObserver = ref(Observer.disposed);
    let connect =
      ObservableSource.publishTo(
        ~onNext=Functions.alwaysUnit,
        ~onComplete=exn => timeOutObserver^ |> Observer.complete(~exn?),
        timeoutObservable,
      );

    let subscribeToTimeout = () => {
      timeoutSubscription |> SerialDisposable.get |> Disposable.dispose;
      timeoutSubscription |> SerialDisposable.set(connect());
    };

    timeOutObserver :=
      Observer.delegate(
        ~onNext=
          next => {
            observer |> Observer.next(next);
            subscribeToTimeout();
          },
        ~onComplete=
          exn => {
            timeoutSubscription |> SerialDisposable.dispose;
            observer |> Observer.complete(~exn?);
          },
        observer,
      )
      |> Observer.addTeardown(() =>
           timeoutSubscription |> SerialDisposable.dispose
         );

    subscribeToTimeout();
    timeOutObserver^;
  };
};

let lift = (scheduler, observable) =>
  observable |> ObservableSource.lift(operator(scheduler));
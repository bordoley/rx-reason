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

    let onNext = next => {
      observer |> Observer.next(next);
      subscribeToTimeout();
    };

    let onComplete = exn => {
      timeoutSubscription |> SerialDisposable.dispose;
      observer |> Observer.complete(~exn?);
    };

    timeOutObserver :=
      observer
      |> Observer.delegate(~onNext, ~onComplete)
      |> Observer.addDisposable(
           SerialDisposable.asDisposable(timeoutSubscription),
         );

    subscribeToTimeout();
    timeOutObserver^;
  };
};

let lift = (scheduler, observable) =>
  observable |> ObservableSource.lift(operator(scheduler));
let operator = scheduler => {
  let timeoutObservable =
    ObservableSource.raise(~scheduler, TimeoutException.Exn);
  subscriber => {
    let timeoutSubscription = SerialDisposable.create();
    let timeOutSubscriber = ref(Subscriber.disposed);
    let connect =
      ObservableSource.publishTo(
        ~onNext=Functions.alwaysUnit,
        ~onComplete=exn => timeOutSubscriber^ |> Subscriber.complete(~exn?),
        timeoutObservable,
      );

    let subscribeToTimeout = () => {
      timeoutSubscription |> SerialDisposable.get |> Disposable.dispose;
      timeoutSubscription |> SerialDisposable.set(connect());
    };

    let onNext = next => {
      subscriber |> Subscriber.next(next);
      subscribeToTimeout();
    };

    let onComplete = exn => {
      timeoutSubscription |> SerialDisposable.dispose;
      subscriber |> Subscriber.complete(~exn?);
    };

    timeOutSubscriber :=
      subscriber
      |> Subscriber.delegate(~onNext, ~onComplete)
      |> Subscriber.addDisposable(
           SerialDisposable.asDisposable(timeoutSubscription),
         );

    subscribeToTimeout();
    timeOutSubscriber^;
  };
};

let lift = (scheduler, observable) =>
  observable |> ObservableSource.lift(operator(scheduler));
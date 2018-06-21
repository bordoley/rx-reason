let operator = {
  let clearDebounce = debounceSubscription =>
    debounceSubscription |> SerialDisposable.set(Disposable.disposed);

  let debounceNext = (debounceSubscription, lastValue, delegate) => {
    clearDebounce(debounceSubscription);
    if (MutableOption.isNotEmpty(lastValue)) {
      let next = MutableOption.get(lastValue);
      MutableOption.unset(lastValue);
      delegate |> Subscriber.next(next);
    };
    Disposable.disposed;
  };

  let onNext = (debounceSubscription, lastValue, scheduler, delegate, next) => {
    clearDebounce(debounceSubscription);
    MutableOption.set(next, lastValue);
    let schedulerDisposable =
      scheduler(() =>
        debounceNext(debounceSubscription, lastValue, delegate)
      );
    debounceSubscription |> SerialDisposable.set(schedulerDisposable);
  };

  let onComplete = (debounceSubscription, lastValue, _, delegate, exn) => {
    switch (exn) {
    | Some(_) => clearDebounce(debounceSubscription)
    | None =>
      debounceNext(debounceSubscription, lastValue, delegate) |> ignore
    };
    delegate |> Subscriber.complete(~exn?);
  };

  (scheduler, subscriber) => {
    let debounceSubscription = SerialDisposable.create();
    let lastValue = MutableOption.create();

    subscriber
    |> Subscriber.delegate3(
         ~onNext,
         ~onComplete,
         debounceSubscription,
         lastValue,
         scheduler,
       )
    |> Subscriber.addSerialDisposable(debounceSubscription);
  };
};

let lift = (scheduler, observable) =>
  observable |> ObservableSource.lift(operator(scheduler));
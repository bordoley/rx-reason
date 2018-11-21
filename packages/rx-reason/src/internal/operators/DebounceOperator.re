let operator = {
  let clearDebounce = debounceSubscription =>
    debounceSubscription
    |> SerialDisposable.setInnerDisposable(Disposable.disposed);

  let debounceNext = (debounceSubscription, lastValue, delegate) => {
    clearDebounce(debounceSubscription);
    if (MutableOption.isNotEmpty(lastValue)) {
      let next = MutableOption.get(lastValue);
      MutableOption.unset(lastValue);
      delegate |> Subscriber.next(next);
    };
  };

  let onDebounceScheduled = (debounceSubscription, lastValue, delegate, _) =>
    debounceNext(debounceSubscription, lastValue, delegate);

  let onNext =
      (debounceSubscription, lastValue, scheduler, dueTime, delegate, next) => {
    clearDebounce(debounceSubscription);
    MutableOption.set(next, lastValue);
    let schedulerDisposable =
      scheduler
      |> Scheduler.scheduleAfter3(
           ~delay=dueTime,
           onDebounceScheduled,
           (),
           debounceSubscription,
           lastValue,
           delegate,
         );

    debounceSubscription
    |> SerialDisposable.setInnerDisposable(schedulerDisposable);
  };

  let onComplete = (debounceSubscription, lastValue, _, _, delegate, exn) => {
    switch (exn) {
    | Some(_) => clearDebounce(debounceSubscription)
    | None => debounceNext(debounceSubscription, lastValue, delegate)
    };
    delegate |> Subscriber.complete(~exn?);
  };

  (~scheduler, ~dueTime, subscriber) => {
    let debounceSubscription = SerialDisposable.create();
    let lastValue = MutableOption.create();

    subscriber
    |> Subscriber.decorate4(
         ~onNext,
         ~onComplete,
         debounceSubscription,
         lastValue,
         scheduler,
         dueTime,
       )
    |> Subscriber.addTeardown1(SerialDisposable.dispose, debounceSubscription);
  };
};
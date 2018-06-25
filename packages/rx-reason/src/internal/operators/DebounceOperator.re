let operator = {
  let clearDebounce = debounceSubscription =>
    debounceSubscription |> SerialDisposable.set(Disposable.disposed);

  let debounceNext = (debounceSubscription, lastValue, delegate, _, _) => {
    clearDebounce(debounceSubscription);
    if (MutableOption.isNotEmpty(lastValue)) {
      let next = MutableOption.get(lastValue);
      MutableOption.unset(lastValue);
      delegate |> Subscriber.next(next);
    };
  };

  let onNext =
      (debounceSubscription, lastValue, scheduler, dueTime, delegate, next) => {
    clearDebounce(debounceSubscription);
    MutableOption.set(next, lastValue);
    let schedulerDisposable =
      scheduler
      |> Scheduler.scheduleAfter3(
           ~delay=dueTime,
           debounceNext,
           (),
           debounceSubscription,
           lastValue,
           delegate,
         );

    debounceSubscription |> SerialDisposable.set(schedulerDisposable);
  };

  let onComplete = (debounceSubscription, lastValue, _, _, delegate, exn) => {
    switch (exn) {
    | Some(_) => clearDebounce(debounceSubscription)
    | None =>
      debounceNext(debounceSubscription, lastValue, delegate) |> ignore
    };
    delegate |> Subscriber.complete(~exn?);
  };

  (scheduler, dueTime, subscriber) => {
    let debounceSubscription = SerialDisposable.create();
    let lastValue = MutableOption.create();

    subscriber
    |> Subscriber.delegate4(
         ~onNext,
         ~onComplete,
         debounceSubscription,
         lastValue,
         scheduler,
         dueTime,
       )
    |> Subscriber.addSerialDisposable(debounceSubscription);
  };
};

let lift = (~scheduler, ~dueTime, observable) =>
  observable |> ObservableSource.lift(operator(scheduler, dueTime));
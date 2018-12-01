let operator = {
  let clearDebounce = debounceSubscription =>
    debounceSubscription
    |> RxSerialDisposable.setInnerDisposable(RxDisposable.disposed);

  let debounceNext = (debounceSubscription, lastValue, delegate) => {
    clearDebounce(debounceSubscription);
    if (RxMutableOption.isNotEmpty(lastValue)) {
      let next = RxMutableOption.get(lastValue);
      RxMutableOption.unset(lastValue);
      delegate |> RxSubscriber.next(next);
    };
  };

  let onDebounceScheduled =
      (
        debounceSubscription,
        lastValue,
        delegate,
        ~now as _,
        ~shouldYield as _,
      ) => {
    debounceNext(debounceSubscription, lastValue, delegate);
    RxScheduler.Result.complete;
  };

  let onNext =
      (debounceSubscription, lastValue, scheduler, dueTime, delegate, next) => {
    clearDebounce(debounceSubscription);
    RxMutableOption.set(next, lastValue);
    let schedulerDisposable =
      scheduler
      |> RxScheduler.schedule(
           ~delay=dueTime,
           onDebounceScheduled(debounceSubscription, lastValue, delegate),
         );

    debounceSubscription
    |> RxSerialDisposable.setInnerDisposable(schedulerDisposable);
  };

  let onComplete = (debounceSubscription, lastValue, _, _, delegate, exn) => {
    switch (exn) {
    | Some(_) => clearDebounce(debounceSubscription)
    | None => debounceNext(debounceSubscription, lastValue, delegate)
    };
    delegate |> RxSubscriber.complete(~exn?);
  };

  (~scheduler, dueTime, subscriber) => {
    let debounceSubscription = RxSerialDisposable.create();
    let lastValue = RxMutableOption.create();

    let debounceDisposable =
      debounceSubscription |> RxSerialDisposable.asDisposable;

    subscriber
    |> RxSubscriber.decorate4(
         ~onNext,
         ~onComplete,
         debounceSubscription,
         lastValue,
         scheduler,
         dueTime,
       )
    |> RxSubscriber.addDisposable(debounceDisposable);
  };
};
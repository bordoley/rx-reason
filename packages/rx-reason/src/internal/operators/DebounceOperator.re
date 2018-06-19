let operator = (scheduler: Scheduler.t) : Operator.t('a, 'a) =>
  subscriber => {
    let lastValue = MutableOption.create();
    let debounceSubscription = SerialDisposable.create();

    let clearDebounce = () =>
      debounceSubscription |> SerialDisposable.set(Disposable.disposed);

    let debouncedNext = () => {
      clearDebounce();
      if (MutableOption.isNotEmpty(lastValue)) {
        let next = MutableOption.get(lastValue);
        MutableOption.unset(lastValue);
        subscriber |> Subscriber.next(next);
      };
      Disposable.disposed;
    };

    let onNext = next => {
      clearDebounce();
      MutableOption.set(next, lastValue);
      debounceSubscription |> SerialDisposable.set(scheduler(debouncedNext));
    };

    let onComplete = exn => {
      switch (exn) {
      | Some(_) => clearDebounce()
      | None => debouncedNext() |> ignore
      };
      subscriber |> Subscriber.complete(~exn?);
    };

    subscriber
    |> Subscriber.delegate(~onNext, ~onComplete)
    |> Subscriber.addDisposable(
         SerialDisposable.asDisposable(debounceSubscription),
       );
  };

let lift = (scheduler, observable) =>
  observable |> ObservableSource.lift(operator(scheduler));
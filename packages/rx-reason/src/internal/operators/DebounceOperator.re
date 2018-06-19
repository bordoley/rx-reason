let operator = (scheduler: Scheduler.t) : Operator.t('a, 'a) =>
  observer => {
    let lastValue = MutableOption.create();
    let debounceSubscription = SerialDisposable.create();

    let clearDebounce = () =>
      debounceSubscription |> SerialDisposable.set(Disposable.disposed);

    let debouncedNext = () => {
      clearDebounce();
      if (MutableOption.isNotEmpty(lastValue)) {
        let next = MutableOption.get(lastValue);
        MutableOption.unset(lastValue);
        observer |> Observer.next(next);
      };
      Disposable.disposed;
    };

    Observer.delegate(
      ~onNext=
        next => {
          clearDebounce();
          MutableOption.set(next, lastValue);
          debounceSubscription
          |> SerialDisposable.set(scheduler(debouncedNext));
        },
      ~onComplete=
        exn => {
          switch (exn) {
          | Some(_) => clearDebounce()
          | None => debouncedNext() |> ignore
          };
          observer |> Observer.complete(~exn?);
        },
      observer,
    )
    |> Observer.addTeardown(() =>
         debounceSubscription |> SerialDisposable.dispose
       );
  };

let lift = (scheduler, observable) =>
  observable |> ObservableSource.lift(operator(scheduler));
type context('a) = {
  debounceSubscription: SerialDisposable.t,
  lastValue: MutableOption.t('a),
  scheduler: Scheduler.t,
};

let operator = {
  let clearDebounce = ({debounceSubscription}) =>
    debounceSubscription |> SerialDisposable.set(Disposable.disposed);

  let debounceNext = ({lastValue} as ctx, delegate) => {
    clearDebounce(ctx);
    if (MutableOption.isNotEmpty(lastValue)) {
      let next = MutableOption.get(lastValue);
      MutableOption.unset(lastValue);
      delegate |> Subscriber.next(next);
    };
    Disposable.disposed;
  };

  let onNext =
      ({debounceSubscription, lastValue, scheduler} as ctx, delegate, next) => {
    clearDebounce(ctx);
    MutableOption.set(next, lastValue);
    let schedulerDisposable = scheduler(() => debounceNext(ctx, delegate));
    debounceSubscription |> SerialDisposable.set(schedulerDisposable);
  };

  let onComplete = (ctx, delegate, exn) => {
    switch (exn) {
    | Some(_) => clearDebounce(ctx)
    | None => debounceNext(ctx, delegate) |> ignore
    };
    delegate |> Subscriber.complete(~exn?);
  };

  (scheduler, subscriber) => {
    let ctx = {
      debounceSubscription: SerialDisposable.create(),
      lastValue: MutableOption.create(),
      scheduler,
    };

    subscriber
    |> Subscriber.delegate(~onNext, ~onComplete, ctx)
    |> Subscriber.addDisposable(
         SerialDisposable.asDisposable(ctx.debounceSubscription),
       );
  };
};

let lift = (scheduler, observable) =>
  observable |> ObservableSource.lift(operator(scheduler));
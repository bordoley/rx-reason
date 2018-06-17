let concat = (~scheduler=Scheduler.immediate, observables) =>
  ObservableSource.create((~onNext, ~onComplete) => {
    let subscription = SerialDisposable.create();

    let rec scheduleSubscription = observables => {
      let newSubscription =
        switch (observables) {
        | [hd, ...tail] =>
          scheduler(() =>
            hd
            |> ObservableSource.subscribeWith(~onNext, ~onComplete=exn =>
                 switch (exn) {
                 | Some(_) => onComplete(exn)
                 | None =>
                   /* Cancel the current subscription here */
                   subscription |> SerialDisposable.set(Disposable.disposed);
                   scheduleSubscription(tail);
                 }
               )
          )
        | [] =>
          onComplete(None);
          Disposable.disposed;
        };

      /* An observable may complete synchronously and continue with an
       * async observable. Avoid canceling the async observable in that case.
       */
      if (! Disposable.isDisposed(newSubscription)) {
        subscription |> SerialDisposable.set(newSubscription);
      };
    };

    scheduleSubscription(observables);
    subscription |> SerialDisposable.asDisposable;
  });
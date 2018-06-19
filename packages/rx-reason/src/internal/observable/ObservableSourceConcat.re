let concat = (~scheduler=Scheduler.immediate, observables) =>
  ObservableSource.createWithObserver(observer => {
    let subscription = SerialDisposable.create();

    let rec scheduleSubscription = observables => {
      let newSubscription =
        switch (observables) {
        | [hd, ...tail] =>
          let onComplete = exn =>
            switch (exn) {
            | Some(_) => observer |> Observer.complete(~exn?)
            | None =>
              /* Cancel the current subscription here */
              subscription |> SerialDisposable.set(Disposable.disposed);
              scheduleSubscription(tail);
            };
            
          scheduler(() =>
            hd
            |> ObservableSource.subscribeWith(
                 ~onNext=Observer.forwardOnNext(observer),
                 ~onComplete,
               )
            |> CompositeDisposable.asDisposable
          );
        | [] =>
          observer |> Observer.complete;
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
    observer
    |> Observer.addDisposable(SerialDisposable.asDisposable(subscription))
    |> ignore;
  });
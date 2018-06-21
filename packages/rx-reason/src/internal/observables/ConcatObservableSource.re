let concat = (~scheduler=Scheduler.immediate, observables) =>
  ObservableSource.create(subscriber => {
    let subscription = SerialDisposable.create();

    let rec scheduleSubscription = observables => {
      let newSubscription =
        switch (observables) {
        | [hd, ...tail] =>
          let onComplete = exn =>
            switch (exn) {
            | Some(_) => subscriber |> Subscriber.complete(~exn?)
            | None =>
              /* Cancel the current subscription here */
              subscription |> SerialDisposable.set(Disposable.disposed);
              scheduleSubscription(tail);
            };

          scheduler(() =>
            hd
            |> ObservableSource.subscribeWith(
                 ~onNext=next => subscriber |> Subscriber.next(next),
                 ~onComplete,
               )
            |> CompositeDisposable.asDisposable
          );
        | [] =>
          subscriber |> Subscriber.complete;
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
    subscriber
    |> Subscriber.addDisposable(SerialDisposable.asDisposable(subscription))
    |> ignore;
  });
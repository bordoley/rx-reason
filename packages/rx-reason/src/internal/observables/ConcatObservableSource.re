let concat = {
  let rec scheduleSubscription =
          (scheduler, observables, subscription, subscriber) => {
    let newSubscription =
      switch (observables) {
      | [hd, ...tail] =>
        scheduler(() =>
          hd
          |> ObservableSource.subscribeWith4(
               ~onNext=Subscriber.delegateOnNext3,
               ~onComplete,
               scheduler,
               tail,
               subscription,
               subscriber,
             )
          |> CompositeDisposable.asDisposable
        )
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
  }
  and onComplete = (scheduler, observables, subscription, subscriber, exn) =>
    switch (exn) {
    | Some(_) as exn => subscriber |> Subscriber.complete(~exn?)
    | None =>
      /* Cancel the current subscription here */
      subscription |> SerialDisposable.set(Disposable.disposed);
      scheduleSubscription(scheduler, observables, subscription, subscriber);
    };

  let source = (scheduler, observables, subscriber) => {
    let subscription = SerialDisposable.create();

    scheduleSubscription(scheduler, observables, subscription, subscriber);
    subscriber |> Subscriber.addSerialDisposable(subscription) |> ignore;
  };

  (~scheduler=Scheduler.immediate, observables) =>
    ObservableSource.create2(source, scheduler, observables);
};
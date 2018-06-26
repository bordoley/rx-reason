let concat = {
  let concatScheduledSource = {
    let onComplete =
        (continuation, observables, innerSubscription, subscriber, exn) =>
      switch (exn) {
      | Some(_) as exn =>
        subscriber |> Subscriber.complete(~exn?);
        continuation |> SchedulerContinuation.dispose;
      | None =>
        innerSubscription
        |> SerialDisposable.setInnerDisposable(Disposable.disposed);
        continuation |> SchedulerContinuation.continue(observables);
      };

    let doWork = (innerSubscription, subscriber, continuation, observables) => {
      let newSubscription =
        switch (observables) {
        | [] =>
          subscriber |> Subscriber.complete;
          Disposable.disposed;
        | [hd, ...tail] =>
          hd
          |> ObservableSource.subscribeWith4(
               ~onNext=Subscriber.delegateOnNext3,
               ~onComplete,
               continuation,
               tail,
               innerSubscription,
               subscriber,
             )
        };

      if (! Disposable.isDisposed(newSubscription)) {
        innerSubscription
        |> SerialDisposable.setInnerDisposable(newSubscription);
      };
    };

    (scheduler, observables, subscriber) => {
      let innerSubscription = SerialDisposable.create();
      let subscription =
        scheduler
        |> Scheduler.schedule2(
             doWork,
             observables,
             innerSubscription,
             subscriber,
           );
      subscriber
      |> Subscriber.addDisposable(subscription)
      |> Subscriber.addTeardown1(SerialDisposable.dispose, innerSubscription)
      |> ignore;
    };
  };

  let concatSynchronousSource = (observables, subscriber) => {
    let innerSubscription = SerialDisposable.create();

    let rec loop = observables => {
      let newSubscription =
        switch (observables) {
        | [] =>
          subscriber |> Subscriber.complete;
          Disposable.disposed;
        | [hd, ...tail] =>
          hd
          |> ObservableSource.subscribeWith1(
               ~onNext=Subscriber.delegateOnNext,
               ~onComplete=
                 (subscriber, exn) =>
                   switch (exn) {
                   | Some(_) as exn =>
                     subscriber |> Subscriber.complete(~exn?)
                   | None =>
                     innerSubscription
                     |> SerialDisposable.setInnerDisposable(
                          Disposable.disposed,
                        );
                     loop(tail);
                   },
               subscriber,
             )
        };

      if (! Disposable.isDisposed(newSubscription)) {
        innerSubscription
        |> SerialDisposable.setInnerDisposable(newSubscription);
      };
    };

    subscriber
    |> Subscriber.addTeardown1(SerialDisposable.dispose, innerSubscription)
    |> ignore;

    loop(observables);
  };

  (~scheduler=?, observables) =>
    switch (scheduler) {
    | Some(scheduler) =>
      ObservableSource.create2(concatScheduledSource, scheduler, observables)
    | None => ObservableSource.create1(concatSynchronousSource, observables)
    };
};
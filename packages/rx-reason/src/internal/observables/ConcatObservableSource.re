let concat = {
  let onComplete =
      (continuation, observables, innerSubscription, subscriber, exn) =>
    switch (exn) {
    | Some(_) as exn =>
      subscriber |> Subscriber.complete(~exn?);
      continuation |> SchedulerContinuation.dispose;
    | None =>
      innerSubscription |> SerialDisposable.set(Disposable.disposed);
      continuation |> SchedulerContinuation.continue(observables);
    };

  let doWork = (innerSubscription, subscriber, continuation, observables) => {
    let newSubscription =
      switch (observables) {
      | [] =>
        subscriber |> Subscriber.complete;
        CompositeDisposable.disposed;
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

    if (! CompositeDisposable.isDisposed(newSubscription)) {
      innerSubscription
      |> SerialDisposable.set(
           CompositeDisposable.asDisposable(newSubscription),
         );
    };
  };

  let source = (scheduler, observables, subscriber) => {
    let innerSubscription = SerialDisposable.create();
    let subscription =
      scheduler
      |> SchedulerNew.schedule2(
           doWork,
           observables,
           innerSubscription,
           subscriber,
         );
    subscriber 
    |> Subscriber.addDisposable(subscription)
    |> Subscriber.addSerialDisposable(innerSubscription) 
    |> ignore;
  };

  (~scheduler=SchedulerNew.immediate, observables) =>
    ObservableSource.create2(source, scheduler, observables);
};
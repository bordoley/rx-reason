let concat = {
  let concatSource = {
    let onComplete =
        (scheduler, tail, loop, innerSubscription, subscriber, exn) =>
      switch (exn) {
      | Some(_) as exn => subscriber |> RxSubscriber.complete(~exn?)
      | None =>
        innerSubscription
        |> RxSerialDisposable.setInnerDisposable(RxDisposable.disposed);
        loop(scheduler, tail, innerSubscription, subscriber);
      };

    let rec loop = (scheduler, observables, innerSubscription, subscriber) => {
      let newSubscription =
        switch (observables) {
        | [] =>
          subscriber |> RxSubscriber.complete;
          RxDisposable.disposed;
        | [hd, ...tail] =>
          let innerSubscriber =
            RxSubscriber.create5(
              ~onNext=RxSubscriber.forwardOnNext4,
              ~onComplete,
              scheduler,
              tail,
              loop,
              innerSubscription,
              subscriber,
            );

          (
            switch (scheduler) {
            | Some(scheduler) =>
              hd |> SubscribeOnObservable.subscribeOn(scheduler)
            | None => hd
            }
          )
          |> RxObservable.subscribeWith(innerSubscriber);

          innerSubscriber |> RxSubscriber.asDisposable;
        };
      if (! RxDisposable.isDisposed(newSubscription)) {
        innerSubscription
        |> RxSerialDisposable.setInnerDisposable(newSubscription);
      };
    };

    (scheduler, observables, subscriber) => {
      let innerSubscription = RxSerialDisposable.create();

      subscriber
      |> RxSubscriber.addTeardown1(
           RxSerialDisposable.dispose,
           innerSubscription,
         )
      |> ignore;

      loop(scheduler, observables, innerSubscription, subscriber);
    };
  };

  (~scheduler=?, observables) =>
    RxObservable.create2(concatSource, scheduler, observables);
};
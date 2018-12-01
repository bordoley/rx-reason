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
          (
            switch (scheduler) {
            | Some(scheduler) =>
              hd |> SubscribeOnObservable.subscribeOn(scheduler)
            | None => hd
            }
          )
          |> RxObservable.observe5(
               ~onNext=SubscriberForward.onNext4,
               ~onComplete,
               scheduler,
               tail,
               loop,
               innerSubscription,
               subscriber,
             )
          |> RxObservable.subscribe
        };
      if (! RxDisposable.isDisposed(newSubscription)) {
        innerSubscription
        |> RxSerialDisposable.setInnerDisposable(newSubscription);
      };
    };

    (scheduler, observables, subscriber) => {
      let innerSubscription = RxSerialDisposable.create();

      let disposable = innerSubscription |> RxSerialDisposable.asDisposable;
      subscriber |> RxSubscriber.addDisposable(disposable) |> ignore;

      loop(scheduler, observables, innerSubscription, subscriber);
    };
  };

  (~scheduler=?, observables) =>
    RxObservable.create2(concatSource, scheduler, observables);
};
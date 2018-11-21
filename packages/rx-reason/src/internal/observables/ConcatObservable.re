let concat = {
  let concatSource = {
    let onComplete =
        (scheduler, tail, loop, innerSubscription, subscriber, exn) =>
      switch (exn) {
      | Some(_) as exn => subscriber |> Subscriber.complete(~exn?)
      | None =>
        innerSubscription
        |> SerialDisposable.setInnerDisposable(Disposable.disposed);
        loop(scheduler, tail, innerSubscription, subscriber);
      };

    let rec loop = (scheduler, observables, innerSubscription, subscriber) => {
      let newSubscription =
        switch (observables) {
        | [] =>
          subscriber |> Subscriber.complete;
          Disposable.disposed;
        | [hd, ...tail] =>
          (
            switch (scheduler) {
            | Some(scheduler) =>
              hd |> SubscribeOnObservable.subscribeOn(scheduler)
            | None => hd
            }
          )
          |> Observable.subscribeWith5(
               ~onNext=Subscriber.forwardOnNext4,
               ~onComplete,
               scheduler,
               tail,
               loop,
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

      subscriber
      |> Subscriber.addTeardown1(SerialDisposable.dispose, innerSubscription)
      |> ignore;

      loop(scheduler, observables, innerSubscription, subscriber);
    };
  };

  (~scheduler=?, observables) =>
    Observable.create2(concatSource, scheduler, observables);
};
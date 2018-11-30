let merge = {
  let onNext = (_, lock, subscriber, next) => {
    RxLock.acquire(lock);
    subscriber |> RxSubscriber.next(next);
    RxLock.release(lock);
  };

  let onComplete = (activeCount, lock, subscriber, exn) => {
    RxLock.acquire(lock);
    let shouldComplete =
      switch (exn) {
      | Some(_) => true
      | None =>
        let oldActiveCount = RxAtomic.decr(activeCount);
        oldActiveCount <= 0;
      };
    if (shouldComplete) {
      subscriber |> RxSubscriber.complete(~exn?);
    };
    RxLock.release(lock);
  };

  let mergeSource = (count, observables, subscriber) => {
    let activeCount = RxAtomic.make(count);
    let lock = RxLock.create();

    let rec loop =
      fun
      | [hd, ...tail] => {
          let innerSubscriber =
            hd
            |> RxObservable.observe3(
                 ~onNext,
                 ~onComplete,
                 activeCount,
                 lock,
                 subscriber,
               )
            |> RxObservable.subscribe;

          subscriber
          |> RxSubscriber.addTeardown1(RxDisposable.dispose, innerSubscriber)
          |> ignore;
          loop(tail);
        }
      | [] => ();

    loop(observables);
  };

  observables => {
    let count = observables |> Lists.length;

    RxObservable.create2(mergeSource, count, observables);
  };
};
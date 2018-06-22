let merge = {
  let onNext = (_, lock, subscriber, next) => {
    Lock.acquire(lock);
    subscriber |> Subscriber.next(next);
    Lock.release(lock);
  };

  let onComplete = (activeCount, lock, subscriber, exn) => {
    Lock.acquire(lock);
    let shouldComplete =
      switch (exn) {
      | Some(_) => true
      | None =>
        let oldActiveCount = Interlocked.decrement(activeCount);
        oldActiveCount <= 0;
      };
    if (shouldComplete) {
      subscriber |> Subscriber.complete(~exn?);
    };
    Lock.release(lock);
  };

  let mergeSource = (count, observables, subscriber) => {
    let activeCount = ref(count);
    let lock = Lock.create();

    let rec loop =
      fun
      | [hd, ...tail] => {
          let subscription =
            ObservableSource.subscribeWith3(
              ~onNext,
              ~onComplete,
              activeCount,
              lock,
              subscriber,
              hd,
            );
          subscriber
          |> Subscriber.addCompositeDisposable(subscription)
          |> ignore;
          loop(tail);
        }
      | [] => ();

    loop(observables);
  };

  observables => {
    let count = observables |> List.length;

    ObservableSource.create2(mergeSource, count, observables);
  };
};
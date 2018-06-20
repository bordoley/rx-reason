let merge = observables => {
  let count = observables |> List.length;

  ObservableSource.create(subscriber => {
    let activeCount = ref(count);
    let lock = Lock.create();
    let onNext = next => {
      Lock.acquire(lock);
      subscriber |> Subscriber.next(next);
      Lock.release(lock);
    };

    let onComplete = exn => {
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

    observables
    |> List.map(ObservableSource.subscribeWith(~onNext, ~onComplete))
    |> List.iter(d =>
         subscriber
         |> Subscriber.addDisposable(CompositeDisposable.asDisposable(d))
         |> ignore
       );
  });
};
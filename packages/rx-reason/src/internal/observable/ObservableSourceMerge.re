let merge = observables => {
  let count = observables |> List.length;

  ObservableSource.createWithObserver(observer => {
    let activeCount = ref(count);
    let lock = Lock.create();
    let onNext = next => {
      Lock.acquire(lock);
      observer |> Observer.next(next);
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
        observer |> Observer.complete(~exn?);
      };
      Lock.release(lock);
    };

    observables
    |> List.map(ObservableSource.subscribeWith(~onNext, ~onComplete))
    |> List.iter(d =>
         observer
         |> Observer.addDisposable(CompositeDisposable.asDisposable(d))
         |> ignore
       );
  });
};
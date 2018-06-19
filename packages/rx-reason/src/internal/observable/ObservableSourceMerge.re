let merge = observables => {
  let count = observables |> List.length;

  ObservableSource.create((~onNext, ~onComplete) => {
    let activeCount = ref(count);
    let lock = Lock.create();
    let subscriptions =
      observables
      |> List.map(
           ObservableSource.subscribeWith(
             ~onNext=
               next => {
                 Lock.acquire(lock);
                 onNext(next);
                 Lock.release(lock);
               },
             ~onComplete=
               exn => {
                 Lock.acquire(lock);
                 let shouldComplete =
                   switch (exn) {
                   | Some(_) => true
                   | None =>
                     let oldActiveCount = Interlocked.decrement(activeCount);
                     oldActiveCount <= 0;
                   };
                 if (shouldComplete) {
                   onComplete(exn);
                 };
                 Lock.release(lock);
               },
           ),
         );
    () => subscriptions |> List.iter(CompositeDisposable.dispose);
  });
};
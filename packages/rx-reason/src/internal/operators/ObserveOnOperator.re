let operator =
    (
      ~bufferStrategy=BufferStrategy.Throw,
      ~bufferSize=(-1),
      scheduler,
      observer,
    ) => {
  let queue =
    QueueWithBufferStrategy.create(~bufferStrategy, ~maxSize=bufferSize);
  let shouldComplete = ref(false);
  let completedState = ref(None);

  let wip = ref(0);
  let innerSubscription =
    Disposable.create(() => {
      Volatile.write(0, wip);
      queue |> QueueWithBufferStrategy.clear;
      completedState := None;
    });

  let rec doWorkStep = () =>
    switch (QueueWithBufferStrategy.tryDeque(queue)) {
    | _ when innerSubscription |> Disposable.isDisposed => Disposable.disposed
    | Some(next) =>
      observer |> Observer.next(next);
      Interlocked.decrement(wip) !== 0 ?
        scheduler(doWorkStep) : Disposable.disposed;
    | _ when Interlocked.exchange(false, shouldComplete) =>
      observer |> Observer.complete(~exn=?completedState^);
      innerSubscription |> Disposable.dispose;
      Disposable.disposed;
    | _ => Disposable.disposed
    };

  let schedule = () =>
    if (Interlocked.increment(wip) === 1) {
      scheduler(doWorkStep) |> ignore;
    };

  Observer.create(
    ~onNext=
      next => {
        queue |> QueueWithBufferStrategy.enqueue(next);
        schedule();
      },
    ~onComplete=
      exn => {
        shouldComplete := true;
        completedState := exn;
        schedule();
      },
    ~onDispose=
      () => {
        innerSubscription |> Disposable.dispose;
        observer |> Observer.dispose;
      },
  );
};

let lift = (~bufferStrategy=?, ~bufferSize=?, scheduler: Scheduler.t, observable) =>
  observable
  |> ObservableSource.lift(
       operator(~bufferStrategy?, ~bufferSize?, scheduler),
     );
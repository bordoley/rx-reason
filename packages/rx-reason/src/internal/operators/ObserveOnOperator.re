let operator = {
  let innerSubscriptionTeardown = (wip, queue, completedState) => {
    Volatile.write(0, wip);
    queue |> QueueWithBufferStrategy.clear;
    completedState := None;
  };

  (
    ~bufferStrategy=BufferStrategy.Throw,
    ~bufferSize=(-1),
    scheduler,
    subscriber,
  ) => {
    let queue =
      QueueWithBufferStrategy.create(~bufferStrategy, ~maxSize=bufferSize);
    let shouldComplete = ref(false);
    let completedState = ref(None);

    let wip = ref(0);
    
    let innerSubscription =
      Disposable.create3(
        innerSubscriptionTeardown,
        wip,
        queue,
        completedState,
      );

    let rec doWorkStep = () =>
      switch (QueueWithBufferStrategy.tryDeque(queue)) {
      | _ when innerSubscription |> Disposable.isDisposed => Disposable.disposed
      | Some(next) =>
        subscriber |> Subscriber.next(next);
        Interlocked.decrement(wip) !== 0 ?
          scheduler(doWorkStep) : Disposable.disposed;
      | _ when Interlocked.exchange(false, shouldComplete) =>
        subscriber |> Subscriber.complete(~exn=?completedState^);
        innerSubscription |> Disposable.dispose;
        Disposable.disposed;
      | _ => Disposable.disposed
      };

    let schedule = () =>
      if (Interlocked.increment(wip) === 1) {
        scheduler(doWorkStep) |> ignore;
      };

    let onNext = (_, next) => {
      queue |> QueueWithBufferStrategy.enqueue(next);
      schedule();
    };

    let onComplete = (_, exn) => {
      shouldComplete := true;
      completedState := exn;
      schedule();
    };

    subscriber
    |> Subscriber.delegate(~onNext, ~onComplete)
    |> Subscriber.addDisposable(innerSubscription);
  };
};

let lift =
    (~bufferStrategy=?, ~bufferSize=?, scheduler: Scheduler.t, observable) =>
  observable
  |> ObservableSource.lift(
       operator(~bufferStrategy?, ~bufferSize?, scheduler),
     );
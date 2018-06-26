let operator = {
  let innerSubscriptionTeardown = (wip, queue, completedState) => {
    Volatile.write(0, wip);
    queue |> QueueWithBufferStrategy.clear;
    completedState := None;
  };

  let doWorkStep =
      (
        innerSubscription,
        delay,
        wip,
        queue,
        shouldComplete,
        completedState,
        subscriber,
        continuation,
        (),
      ) =>
    switch (QueueWithBufferStrategy.tryDeque(queue)) {
    | _ when innerSubscription |> Disposable.isDisposed => ()
    | Some(next) =>
      subscriber |> Subscriber.next(next);
      Interlocked.decrement(wip) !== 0 ?
        continuation |> SchedulerContinuation.continueAfter(~delay, ()) : ();
    | _ when Interlocked.exchange(false, shouldComplete) =>
      subscriber |> Subscriber.complete(~exn=?completedState^);
      innerSubscription |> Disposable.dispose;
    | _ => ()
    };

  let schedule =
      (
        scheduler,
        delay,
        innerSubscription,
        wip,
        queue,
        shouldComplete,
        completedState,
        subscriber,
      ) =>
    if (Interlocked.increment(wip) === 1) {
      scheduler
      |> Scheduler.scheduleAfter7(
           ~delay,
           doWorkStep,
           (),
           innerSubscription,
           delay,
           wip,
           queue,
           shouldComplete,
           completedState,
           subscriber,
         )
      |> ignore;
    };

  let onNext =
      (
        scheduler,
        delay,
        innerSubscription,
        wip,
        queue,
        shouldComplete,
        completedState,
        subscriber,
        next,
      ) => {
    queue |> QueueWithBufferStrategy.enqueue(next);
    schedule(
      scheduler,
      delay,
      innerSubscription,
      wip,
      queue,
      shouldComplete,
      completedState,
      subscriber,
    );
  };

  let onComplete =
      (
        scheduler,
        delay,
        innerSubscription,
        wip,
        queue,
        shouldComplete,
        completedState,
        subscriber,
        exn,
      ) => {
    shouldComplete := true;
    completedState := exn;
    schedule(
      scheduler,
      delay,
      innerSubscription,
      wip,
      queue,
      shouldComplete,
      completedState,
      subscriber,
    );
  };

  (bufferStrategy, bufferSize, scheduler, delay, subscriber) => {
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

    subscriber
    |> Subscriber.decorate7(
         ~onNext,
         ~onComplete,
         scheduler,
         delay,
         innerSubscription,
         wip,
         queue,
         shouldComplete,
         completedState,
       )
    |> Subscriber.addTeardown1(Disposable.dispose, innerSubscription);
  };
};

let lift =
    (
      ~bufferStrategy=BufferStrategy.Raise,
      ~bufferSize=(-1),
      ~scheduler,
      ~delay,
      observable,
    ) =>
  observable
  |> ObservableSource.lift(
       operator(bufferStrategy, bufferSize, scheduler, delay),
     );
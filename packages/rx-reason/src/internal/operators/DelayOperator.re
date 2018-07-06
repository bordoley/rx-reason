let operator = {
  let innerSubscriptionTeardown = (wip, queue, completedState) => {
    Atomic.set(wip, 0);
    queue |> QueueWithBufferStrategy.dispose;
    Atomic.set(completedState, None);
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
        (),
      ) =>
    switch (QueueWithBufferStrategy.tryDeque(queue)) {
    | _ when innerSubscription |> Disposable.isDisposed => ScheduledWork.Result.Done
    | Some(next) =>
      subscriber |> Subscriber.next(next);
      Atomic.decr(wip) !== 0 ?
        ScheduledWork.Result.ContinueAfter(delay, ()) : ScheduledWork.Result.Done;
    | _ when Atomic.exchange(shouldComplete, false) =>
      subscriber |> Subscriber.complete(~exn=?Atomic.get(completedState));
      innerSubscription |> Disposable.dispose;
      ScheduledWork.Result.Done;
    | _ => ScheduledWork.Result.Done
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
    if (Atomic.incr(wip) === 1) {
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
    Atomic.set(shouldComplete, true);
    Atomic.set(completedState, exn);
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
    let shouldComplete = Atomic.make(false);
    let completedState = Atomic.make(None);
    let wip = Atomic.make(0);

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
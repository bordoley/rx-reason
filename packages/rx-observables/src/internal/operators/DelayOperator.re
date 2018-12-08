let schedule = (wip, doWork, queue, delay, notification, scheduler) => {
  let now = scheduler |> RxScheduler.now;
  let dueTime = now +. delay;
  queue |> RxMutableQueue.enqueue((dueTime, notification));
  if (RxAtomic.incr(wip) === 1) {
    scheduler |> RxScheduler.schedule(~delay, doWork) |> ignore;
  };
};

let onNext = (scheduler, wip, doWork, queue, delay, _, next) =>
  scheduler |> schedule(wip, doWork, queue, delay, RxNotification.next(next));

let onComplete = (scheduler, wip, doWork, queue, delay, _, exn) =>
  scheduler
  |> schedule(wip, doWork, queue, delay, RxNotification.complete(exn));

let create = (~scheduler, delay, subscriber) => {
  RxPreconditions.checkArgument(
    delay > 0.0,
    "DelayOperator: delay must be greater than 0.0 milliseconds",
  );

  let queue = RxMutableQueue.create();
  let wip = RxAtomic.make(0);

  let rec doWork = (~now, ~shouldYield) => {
    let currentTime = now();
    let nextDelay =
      switch (RxMutableQueue.peek(queue)) {
      | Some((dueTime, notification)) when currentTime >= dueTime =>
        RxMutableQueue.dequeue(queue) |> ignore;
        subscriber |> RxSubscriber.notify(notification);
        0.0;
      | Some((dueTime, _)) => dueTime -. currentTime
      | _ => (-1.0)
      };

    let yieldRequested = shouldYield();
    if (nextDelay > 0.0) {
      RxScheduler.Result.continueAfter(~delay=nextDelay, doWork);
    } else if (nextDelay < 0.0) {
      if (RxAtomic.decr(wip) !== 0) {
        RxScheduler.Result.yield(doWork);
      } else {
        RxScheduler.Result.complete;
      };
    } else if (yieldRequested) {
      RxScheduler.Result.yield(doWork);
    } else {
      doWork(~now, ~shouldYield);
    };
  };

  subscriber
  |> RxSubscriber.decorate5(
       ~onNext,
       ~onComplete,
       scheduler,
       wip,
       doWork,
       queue,
       delay,
     );
};
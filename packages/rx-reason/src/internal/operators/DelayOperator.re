let schedule = (wip, doWork, queue, delay, notification, scheduler) => {
  let now = scheduler |> Scheduler.now;
  let dueTime = now +. delay;
  queue |> MutableQueue.enqueue((dueTime, notification));
  if (Atomic.incr(wip) === 1) {
    scheduler |> Scheduler.schedule(~delay, doWork) |> ignore;
  };
};

let onNext = (scheduler, wip, doWork, queue, delay, _, next) =>
  scheduler |> schedule(wip, doWork, queue, delay, Notification.Next(next));

let onComplete = (scheduler, wip, doWork, queue, delay, _, exn) =>
  scheduler
  |> schedule(wip, doWork, queue, delay, Notification.complete(exn));

let operator = (~scheduler, ~delay, subscriber) => {
  let queue = MutableQueue.create();
  let wip = Atomic.make(0);

  let rec doWork = (~now, ~shouldYield) => {
    let currentTime = now();
    let nextDelay =
      switch (MutableQueue.peek(queue)) {
      | Some((dueTime, notification)) when currentTime >= dueTime =>
        MutableQueue.dequeue(queue) |> ignore;
        subscriber |> Subscriber.notify(notification);
        0.0;
      | Some((dueTime, _)) => dueTime -. currentTime
      | _ => (-1.0)
      };

    let yieldRequested = shouldYield();
    if (nextDelay > 0.0) {
      Scheduler.Result.continueAfter(~delay=nextDelay, doWork);
    } else if (nextDelay < 0.0) {
      if (Atomic.decr(wip) !== 0) {
        Scheduler.Result.yield(doWork);
      } else {
        Scheduler.Result.complete;
      };
    } else if (! yieldRequested) {
      doWork(~now, ~shouldYield);
    } else {
      Scheduler.Result.yield(doWork);
    };
  };

  subscriber
  |> Subscriber.decorate5(
       ~onNext,
       ~onComplete,
       scheduler,
       wip,
       doWork,
       queue,
       delay,
     );
};
let schedule = (wip, doWork, queue, notification, scheduler) => {
  queue |> MutableQueue.enqueue(notification);
  if (Atomic.incr(wip) === 1) {
    scheduler |> Scheduler.schedule(doWork) |> ignore;
  };
};

let onNext = (scheduler, wip, doWork, queue, _, next) =>
  scheduler |> schedule(wip, doWork, queue, Notification.Next(next));

let onComplete = (scheduler, wip, doWork, queue, _, exn) =>
  scheduler
  |> schedule(wip, doWork, queue, Notification.complete(exn));

let operator = (scheduler, subscriber) => {
  let queue = MutableQueue.create();
  let wip = Atomic.make(0);

  let rec doWork = (~now, ~shouldYield) => {
    let loopAgain =
      switch (MutableQueue.dequeue(queue)) {
      | Some(notification) =>
        subscriber |> Subscriber.notify(notification);
        true;
      | _ => false
      };

    let yieldRequested = shouldYield();
    if (loopAgain && ! yieldRequested) {
      doWork(~now, ~shouldYield);
    } else if (Atomic.decr(wip) !== 0) {
      Scheduler.Result.yield(doWork);
    } else {
      Scheduler.Result.complete;
    };
  };

  subscriber
  |> Subscriber.decorate4(~onNext, ~onComplete, scheduler, wip, doWork, queue);
};
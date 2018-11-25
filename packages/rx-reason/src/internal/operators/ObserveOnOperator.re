let schedule = (wip, doWork, scheduler) =>
  if (Atomic.incr(wip) === 1) {
    scheduler |> Scheduler.schedule(doWork) |> ignore;
  };

let onNext = (scheduler, wip, doWork, queue, _, next) => {
  queue |> MutableQueue.enqueue(Notification.Next(next));
  scheduler |> schedule(wip, doWork);
};

let onComplete = (scheduler, wip, doWork, queue, _, exn) => {
  queue |> MutableQueue.enqueue(Notification.Complete(exn));
  scheduler |> schedule(wip, doWork);
};

let operator = (scheduler, subscriber) => {
  let queue = MutableQueue.create();
  let wip = Atomic.make(0);

  let rec doWork = (~now, ~shouldYield) => {
    let loopAgain =
      switch (MutableQueue.dequeue(queue)) {
      | Some(Notification.Next(next)) =>
        subscriber |> Subscriber.next(next);
        true;
      | Some(Notification.Complete(exn)) =>
        subscriber |> Subscriber.complete(~exn?);
        true;
      | _ => false
      };

    let yieldRequested = shouldYield();
    if (loopAgain && ! yieldRequested) {
      doWork(~now, ~shouldYield);
    } else {
      Atomic.decr(wip) !== 0 ?
        Scheduler.Result.yield(doWork) : Scheduler.Result.complete;
    };
  };

  subscriber
  |> Subscriber.decorate4(~onNext, ~onComplete, scheduler, wip, doWork, queue);
};
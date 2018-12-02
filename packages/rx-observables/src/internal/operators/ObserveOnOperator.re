let schedule = (wip, doWork, queue, notification, scheduler) => {
  queue |> RxMutableQueue.enqueue(notification);
  if (RxAtomic.incr(wip) === 1) {
    scheduler |> RxScheduler.schedule(doWork) |> ignore;
  };
};

let onNext = (scheduler, wip, doWork, queue, _, next) =>
  scheduler |> schedule(wip, doWork, queue, RxNotification.Next(next));

let onComplete = (scheduler, wip, doWork, queue, _, exn) =>
  scheduler |> schedule(wip, doWork, queue, RxNotification.complete(exn));

let operator = (scheduler, subscriber) => {
  let queue = RxMutableQueue.create();
  let wip = RxAtomic.make(0);

  let rec doWork = (~now, ~shouldYield) => {
    let loopAgain =
      switch (RxMutableQueue.dequeue(queue)) {
      | Some(RxNotification.Next(v)) =>
        subscriber |> RxSubscriber.next(v);
        true;
      | Some(RxNotification.Complete(exn)) =>
        subscriber |> RxSubscriber.complete(~exn?);
        true;
      | _ => false
      };

    let yieldRequested = shouldYield();
    if (loopAgain && ! yieldRequested) {
      doWork(~now, ~shouldYield);
    } else if (loopAgain) {
      RxScheduler.Result.yield(doWork);
    } else if (RxAtomic.decr(wip) !== 0) {
      RxScheduler.Result.yield(doWork);
    } else {
      RxScheduler.Result.complete;
    };
  };

  subscriber
  |> RxSubscriber.decorate4(
       ~onNext,
       ~onComplete,
       scheduler,
       wip,
       doWork,
       queue,
     );
};
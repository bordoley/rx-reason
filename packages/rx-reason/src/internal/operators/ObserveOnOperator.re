let forwardOnNext = (subscriber, next, ~now as _, ~shouldYield as _) => {
  subscriber |> Subscriber.next(next);
  Scheduler.Result.complete;
};

let onNext = (scheduler, subscriber, next) =>
  scheduler |> Scheduler.schedule(forwardOnNext(subscriber, next)) |> ignore;

let forwardOnComplete = (subscriber, exn, ~now as _, ~shouldYield as _) => {
  subscriber |> Subscriber.complete(~exn?);
  Scheduler.Result.complete;
};

let onComplete = (scheduler, subscriber, exn) =>
  scheduler
  |> Scheduler.schedule(forwardOnComplete(subscriber, exn))
  |> ignore;

let operator = (scheduler, subscriber) =>
  subscriber |> Subscriber.decorate1(~onNext, ~onComplete, scheduler);
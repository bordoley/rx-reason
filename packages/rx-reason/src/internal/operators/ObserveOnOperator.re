let onNext = (scheduler, subscriber, next) => {
  scheduler |> Scheduler.schedule2(Subscriber.forwardOnNext, subscriber, next) |> ignore;
};

let onComplete = (scheduler, subscriber, exn) => {
  scheduler |> Scheduler.schedule2(Subscriber.forwardOnComplete, subscriber, exn) |> ignore;
};

let operator = (scheduler, subscriber) => 
   subscriber |> Subscriber.decorate1(~onNext, ~onComplete, scheduler);
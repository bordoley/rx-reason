let ofList = {
  let ofListSynchronousSource = (list, subscriber) => {
    let rec loop = list =>
      switch (list) {
      | [hd, ...tail] =>
        subscriber |> Subscriber.next(hd);
        loop(tail);
      | [] => subscriber |> Subscriber.complete
      };
    loop(list);
  };

  let ofListScheduledSource = (scheduler, list, subscriber) => {
    let loop = (continuation, list) =>
      switch (list) {
      | [hd] =>
        subscriber |> Subscriber.next(hd);
        subscriber |> Subscriber.complete;
        continuation |> SchedulerContinuation.dispose;
      | [hd, ...tail] =>
        subscriber |> Subscriber.next(hd);
        continuation |> SchedulerContinuation.continue(tail);
      | [] =>
        subscriber |> Subscriber.complete;
        continuation |> SchedulerContinuation.dispose;
      };

    let schedulerSubscription = scheduler |> Scheduler.schedule(loop, list);
    subscriber
    |> Subscriber.addTeardown1(Disposable.dispose, schedulerSubscription)
    |> ignore;
  };

  (~scheduler=?, list) =>
    switch (scheduler) {
    | Some(scheduler) =>
      ObservableSource.create2(ofListScheduledSource, scheduler, list)
    | None => ObservableSource.create1(ofListSynchronousSource, list)
    };
};

let ofNotifications = (~scheduler=?, notifications) =>
  ofList(~scheduler?, notifications) |> DematerializeOperator.lift;

let ofValue = {
  let ofValueSynchronousSource = (value, subscriber) => {
    subscriber |> Subscriber.next(value);
    subscriber |> Subscriber.complete;
  };

  (~scheduler=?, value) =>
    switch (scheduler) {
    | Some(scheduler) => ofList(~scheduler, [value])
    | None => ObservableSource.create1(ofValueSynchronousSource, value)
    };
};

let ofRelativeTimeNotifications = {
  let ofRelativeTimeNotificationsScheduledSource = {
    let loop =
        (subscriber, continuation, (notif, previousDelay, notifications)) => {
      subscriber |> Subscriber.notify(notif);

      switch (notifications) {
      | [(requestedDelay, notif), ...tail] =>
        let computedDelay = max(0.0, requestedDelay -. previousDelay);

        continuation
        |> SchedulerContinuation.continueAfter(
             ~delay=computedDelay,
             (notif, requestedDelay, tail),
           );
      | [] => continuation |> SchedulerContinuation.dispose
      };
    };

    (scheduler, notifications, subscriber) =>
      switch (notifications) {
      | [(delay, notif), ...tail] =>
        let schedulerSubscription =
          scheduler
          |> Scheduler.scheduleAfter1(
               ~delay,
               loop,
               (notif, delay, tail),
               subscriber,
             );
        subscriber
        |> Subscriber.addTeardown1(Disposable.dispose, schedulerSubscription)
        |> ignore;
      | [] => ()
      };
  };

  (~scheduler, notifications) =>
    ObservableSource.create2(
      ofRelativeTimeNotificationsScheduledSource,
      scheduler,
      notifications,
    );
};

let ofAbsoluteTimeNotifications = (~scheduler, notifications) => {
  let currentTime = scheduler |> Scheduler.now;

  let relativeTimeNotifications =
    notifications
    |> Lists.mapTailRecursive(((time, notif)) =>
         (time -. currentTime, notif)
       );

  ofRelativeTimeNotifications(~scheduler, relativeTimeNotifications);
};
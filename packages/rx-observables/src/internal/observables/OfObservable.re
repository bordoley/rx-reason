let ofList = {
  let ofListSynchronousSource = (list, subscriber) => {
    let rec loop = list =>
      switch (list) {
      | [hd, ...tail] =>
        subscriber |> RxSubscriber.next(hd);
        loop(tail);
      | [] => subscriber |> RxSubscriber.complete
      };
    loop(list);
  };

  let ofListScheduledSource = (scheduler, list, subscriber) => {
    let rec loop = (list, ~now, ~shouldYield) =>
      switch (list) {
      | [hd] =>
        subscriber |> RxSubscriber.next(hd);
        subscriber |> RxSubscriber.complete;
        RxScheduler.Result.complete;
      | [hd, ...tail] =>
        subscriber |> RxSubscriber.next(hd);

        /* Keep pushing values until told to yield */
        if (shouldYield()) {
          RxScheduler.Result.yield(loop(tail));
        } else {
          loop(tail, ~now, ~shouldYield);
        };
      | [] =>
        subscriber |> RxSubscriber.complete;
        RxScheduler.Result.complete;
      };

    let schedulerSubscription =
      scheduler |> RxScheduler.schedule(~delay=0.0, loop(list));
    subscriber |> RxSubscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (~scheduler=?, list) =>
    switch (scheduler) {
    | Some(scheduler) =>
      RxObservable.create2(ofListScheduledSource, scheduler, list)
    | None => RxObservable.create1(ofListSynchronousSource, list)
    };
};

let ofNotifications = (~scheduler=?, notifications) =>
  ofList(~scheduler?, notifications)
  |> RxObservable.lift(DematerializeOperator.operator);

let ofValue = {
  let ofValueSynchronousSource = (value, subscriber) => {
    subscriber |> RxSubscriber.next(value);
    subscriber |> RxSubscriber.complete;
  };

  (~scheduler=?, value) => {
    let source = RxObservable.create1(ofValueSynchronousSource, value);

    switch (scheduler) {
    | Some(scheduler) =>
      source |> SubscribeOnObservable.subscribeOn(scheduler)
    | None => source
    };
  };
};

let ofRelativeTimeNotifications = {
  let ofRelativeTimeNotificationsScheduledSource = {
    let rec loop =
            (
              subscriber,
              notif,
              previousDelay,
              notifications,
              ~now as _,
              ~shouldYield as _,
            ) => {
              
      switch (notif) {
        | RxNotification.Next(v) => subscriber |> RxSubscriber.next(v)
        | RxNotification.Complete(exn) =>
          subscriber |> RxSubscriber.complete(~exn?)
        };        

      switch (notifications) {
      | [(requestedDelay, notif), ...tail] =>
        let computedDelay = max(0.0, requestedDelay -. previousDelay);

        RxScheduler.Result.continueAfter(
          ~delay=computedDelay,
          loop(subscriber, notif, requestedDelay, tail),
        );
      | [] => RxScheduler.Result.complete
      };
    };

    (scheduler, notifications, subscriber) =>
      switch (notifications) {
      | [(delay, notif), ...tail] =>
        let schedulerSubscription =
          scheduler
          |> RxScheduler.schedule(
               ~delay,
               loop(subscriber, notif, delay, tail),
             );
        subscriber
        |> RxSubscriber.addDisposable(schedulerSubscription)
        |> ignore;
      | [] => ()
      };
  };

  (~scheduler, notifications) =>
    RxObservable.create2(
      ofRelativeTimeNotificationsScheduledSource,
      scheduler,
      notifications,
    );
};

let ofAbsoluteTimeNotifications = (~scheduler, notifications) => {
  let currentTime = scheduler |> RxScheduler.now;

  let relativeTimeNotifications =
    notifications
    |> Lists.mapTailRecursive(((time, notif)) =>
         (time -. currentTime, notif)
       );

  ofRelativeTimeNotifications(~scheduler, relativeTimeNotifications);
};
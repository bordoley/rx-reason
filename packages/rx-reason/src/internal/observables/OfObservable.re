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
    let rec loop = (list, ~shouldYield as _) =>
      switch (list) {
      | [hd] =>
        subscriber |> Subscriber.next(hd);
        subscriber |> Subscriber.complete;
        Scheduler.Result.complete;
      | [hd, ...tail] =>
        /* FIXME: keep tail recursing until shouldYield is true */
        subscriber |> Subscriber.next(hd);
        Scheduler.Result.yield(loop(tail));
      | [] =>
        subscriber |> Subscriber.complete;
        Scheduler.Result.complete;
      };

    let schedulerSubscription =
      scheduler |> Scheduler.schedule(~delay=0.0, loop(list));
    subscriber
    |> Subscriber.addTeardown1(Disposable.dispose, schedulerSubscription)
    |> ignore;
  };

  (~scheduler=?, list) =>
    switch (scheduler) {
    | Some(scheduler) =>
      Observable.create2(ofListScheduledSource, scheduler, list)
    | None => Observable.create1(ofListSynchronousSource, list)
    };
};

let ofNotifications = (~scheduler=?, notifications) =>
  ofList(~scheduler?, notifications)
  |> Observable.lift(DematerializeOperator.operator);

let ofValue = {
  let ofValueSynchronousSource = (value, subscriber) => {
    subscriber |> Subscriber.next(value);
    subscriber |> Subscriber.complete;
  };

  (~scheduler=?, value) => {
    let source = Observable.create1(ofValueSynchronousSource, value);

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
              ~shouldYield as _,
            ) => {
      subscriber |> Subscriber.notify(notif);

      switch (notifications) {
      | [(requestedDelay, notif), ...tail] =>
        let computedDelay = max(0.0, requestedDelay -. previousDelay);

        Scheduler.Result.continueAfter(
          ~delay=computedDelay,
          loop(subscriber, notif, requestedDelay, tail),
        );
      | [] => Scheduler.Result.complete
      };
    };

    (scheduler, notifications, subscriber) =>
      switch (notifications) {
      | [(delay, notif), ...tail] =>
        let schedulerSubscription =
          scheduler
          |> Scheduler.schedule(~delay, loop(subscriber, notif, delay, tail));
        subscriber
        |> Subscriber.addTeardown1(Disposable.dispose, schedulerSubscription)
        |> ignore;
      | [] => ()
      };
  };

  (~scheduler, notifications) =>
    Observable.create2(
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
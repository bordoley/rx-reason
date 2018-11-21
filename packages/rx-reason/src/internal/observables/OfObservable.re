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
    let loop = list =>
      switch (list) {
      | [hd] =>
        subscriber |> Subscriber.next(hd);
        subscriber |> Subscriber.complete;
        Scheduler.RecursiveResult.done_;
      | [hd, ...tail] =>
        subscriber |> Subscriber.next(hd);
        Scheduler.RecursiveResult.continue(tail);
      | [] =>
        subscriber |> Subscriber.complete;
        Scheduler.RecursiveResult.done_;
      };

    let schedulerSubscription = scheduler |> Scheduler.scheduleRecursive(~delay=0.0, loop, list);
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
  ofList(~scheduler?, notifications) |> Observable.lift(DematerializeOperator.operator);

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
    let loop = (subscriber, (notif, previousDelay, notifications)) => {
      subscriber |> Subscriber.notify(notif);

      switch (notifications) {
      | [(requestedDelay, notif), ...tail] =>
        let computedDelay = max(0.0, requestedDelay -. previousDelay);

        Scheduler.RecursiveResult.continueAfter(
          ~delay=computedDelay,
          (notif, requestedDelay, tail),
        );
      | [] => Scheduler.RecursiveResult.done_
      };
    };

    (scheduler, notifications, subscriber) =>
      switch (notifications) {
      | [(delay, notif), ...tail] =>
        let schedulerSubscription =
          scheduler
          |> Scheduler.scheduleRecursive1(
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
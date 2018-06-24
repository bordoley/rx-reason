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

    let schedulerSubscription =
      scheduler |> SchedulerNew.schedule(loop, list);
    subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (~scheduler=SchedulerNew.immediate, list) =>
    scheduler === SchedulerNew.immediate ?
      ObservableSource.create1(ofListSynchronousSource, list) :
      ObservableSource.create2(ofListScheduledSource, scheduler, list);
};

let ofNotifications = {
  let ofNotificationsSynchronousSource = (notifications, subscriber) => {
    let rec loop = list =>
      switch (list) {
      | [hd, ...tail] =>
        subscriber |> Subscriber.notify(hd);
        loop(tail);
      | [] => ()
      };
    loop(notifications);
  };

  let ofNotificationsScheduledSource = {
    let loop = (subscriber, continuation, notifications) =>
      switch (notifications) {
      | [hd] =>
        subscriber |> Subscriber.notify(hd);
        continuation |> SchedulerContinuation.dispose;
      | [hd, ...tail] =>
        subscriber |> Subscriber.notify(hd);
        continuation |> SchedulerContinuation.continue(tail);
      | [] => continuation |> SchedulerContinuation.dispose
      };

    (scheduler, notifications, subscriber) => {
      let schedulerSubscription =
        scheduler |> SchedulerNew.schedule1(loop, notifications, subscriber);
      subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
    };
  };

  (~scheduler=SchedulerNew.immediate, notifications) =>
    scheduler === SchedulerNew.immediate ?
      ObservableSource.create1(
        ofNotificationsSynchronousSource,
        notifications,
      ) :
      ObservableSource.create2(
        ofNotificationsScheduledSource,
        scheduler,
        notifications,
      );
};

let ofValue = {
  let ofValueSynchronousSource = (value, subscriber) => {
    subscriber |> Subscriber.next(value);
    subscriber |> Subscriber.complete;
  };

  (~scheduler=SchedulerNew.immediate, value) =>
    scheduler === SchedulerNew.immediate ?
      ObservableSource.create1(ofValueSynchronousSource, value) :
      ofList(~scheduler, [value]);
};

let ofRelativeTimeNotifications = {
  let ofRelativeTimeNotificationsScheduledSource = {
    let loop =
      (subscriber, continuation, (notif, previousDelay, notifications)) => {
        subscriber |> Subscriber.notify(notif);

        switch (notifications) {
        | [(delay, notif), ...tail] =>
          let delay = max(0.0, delay -. previousDelay);

          continuation
          |> DelayableSchedulerContinuation.continue(
               ~delay,
               (notif, delay, tail, ),
             );
        | [] => continuation |> DelayableSchedulerContinuation.dispose
        };
      };

    (scheduler, notifications, subscriber) =>
      switch (notifications) {
      | [(delay, notif), ...tail] =>
        let schedulerSubscription =
          scheduler
          |> TimeScheduler.scheduleAfter1(
               ~delay,
               loop,
               (notif, delay, tail),
               subscriber,
             );
        subscriber
        |> Subscriber.addDisposable(schedulerSubscription)
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

let ofAbsoluteTimeNotifications =
  (~scheduler, notifications) => {
    let currentTime = scheduler |> TimeScheduler.now;

    let relativeTimeNotifications = notifications |> List.rev_map(
      ((time, notif)) => (time -. currentTime, notif)
    );

    ofRelativeTimeNotifications(~scheduler, relativeTimeNotifications);
};

let ofAbsoluteTimeNotifications = {
  let ofAbsoluteTimeNotificationSource =
      (scheduler: ClockScheduler.t, notifications, subscriber) => {
    let now = scheduler.now;
    let scheduleWithDelay = scheduler.scheduleWithDelay;
    let rec loop = lst =>
      switch (lst) {
      | [(time, notif), ...tail] =>
        let delay = time -. now();
        if (delay > 0.0) {
          scheduleWithDelay(
            delay,
            () => {
              subscriber |> Subscriber.notify(notif);
              loop(tail);
            },
          );
        } else {
          loop(tail);
        };
      | [] => Disposable.disposed
      };

    let loopSubscription = loop(notifications);
    subscriber |> Subscriber.addDisposable(loopSubscription) |> ignore;
  };

  (~scheduler, notifications) =>
    ObservableSource.create2(
      ofAbsoluteTimeNotificationSource,
      scheduler,
      notifications,
    );
};

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
    let rec loop = (list, ()) =>
      switch (list) {
      | [hd] =>
        subscriber |> Subscriber.next(hd);
        subscriber |> Subscriber.complete;
        Disposable.disposed;
      | [hd, ...tail] =>
        subscriber |> Subscriber.next(hd);
        scheduler(loop(tail));
      | [] =>
        subscriber |> Subscriber.complete;
        Disposable.disposed;
      };

    let schedulerSubscription = scheduler(loop(list));
    subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (~scheduler=Scheduler.immediate, list) =>
    scheduler === Scheduler.immediate ?
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

  let ofNotificationsScheduledSource = (scheduler, notifications, subscriber) => {
    let rec loop = (list, ()) =>
      switch (list) {
      | [hd] =>
        subscriber |> Subscriber.notify(hd);
        Disposable.disposed;
      | [hd, ...tail] =>
        subscriber |> Subscriber.notify(hd);
        scheduler(loop(tail));
      | [] => Disposable.disposed
      };
    let schedulerSubscription = scheduler(loop(notifications));
    subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (~scheduler=Scheduler.immediate, notifications) =>
    scheduler === Scheduler.immediate ?
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

  let ofValueScheduledSource = (scheduler, value, subscriber) => {
    let schedulerSubscription =
      scheduler(() => {
        subscriber |> Subscriber.next(value);
        scheduler(() => {
          subscriber |> Subscriber.complete;
          Disposable.disposed;
        });
      });

    subscriber |> Subscriber.addDisposable(schedulerSubscription) |> ignore;
  };

  (~scheduler=Scheduler.immediate, value) =>
    scheduler === Scheduler.immediate ?
      ObservableSource.create1(ofValueSynchronousSource, value) :
      ObservableSource.create2(ofValueScheduledSource, scheduler, value);
};

let ofRelativeTimeNotifications = {
  let source = (scheduler, notifications, subscriber) => {
    let rec loop = (lst, previousDelay) =>
      switch (lst) {
      | [(delay, notif), ...tail] =>
        scheduler(
          max(0.0, delay -. previousDelay),
          () => {
            subscriber |> Subscriber.notify(notif);
            loop(tail, delay);
          },
        )
      | [] => Disposable.disposed
      };

    let loopSubscription = loop(notifications, 0.0);
    subscriber |> Subscriber.addDisposable(loopSubscription) |> ignore;
  };

  (~scheduler, notifications) =>
    ObservableSource.create2(source, scheduler, notifications);
};
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
      scheduler |> RxScheduler.schedule(loop(list));
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
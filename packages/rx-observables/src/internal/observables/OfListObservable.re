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
    if (RxSubscriber.isDisposed(subscriber)) {
      RxScheduler.Result.complete;
    } else {
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
    };

  let schedulerSubscription = scheduler |> RxScheduler.schedule(loop(list));
  subscriber |> RxSubscriber.addDisposable(schedulerSubscription) |> ignore;
};

let create = (~scheduler=?, list) =>
  switch (scheduler) {
  | Some(scheduler) =>
    RxObservable.create2(ofListScheduledSource, scheduler, list)
  | None => RxObservable.create1(ofListSynchronousSource, list)
  };
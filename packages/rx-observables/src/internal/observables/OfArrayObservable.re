let ofArraySynchronousSource = (array, subscriber) => {
  for (i in 0 to Js.Array.length(array) - 1) {
    subscriber |> RxSubscriber.next(array[i]);
  };
  subscriber |> RxSubscriber.complete;
};
/*
let ofListScheduledSource = (scheduler, list, subscriber) => {
  let rec loop = (list, ~now, ~shouldYield) =>
    if (RxSubscriber.isDisposed(subscriber)) {
      RxScheduler.Result.complete;
    } else {
      switch (list) {
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
};*/

let create = (~scheduler=?, array) =>
  switch (scheduler) {
  | Some(scheduler) => RxObservable.create1(ofArraySynchronousSource, array)
  | None => RxObservable.create1(ofArraySynchronousSource, array)
  };
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

  (~scheduler=Scheduler.immediate, list: list('a)) =>
    scheduler === Scheduler.immediate ?
      ObservableSource.create1(ofListSynchronousSource, list) :
      ObservableSource.create2(ofListScheduledSource, scheduler, list);
};
let onNext = (last, _, next) => RxMutableOption.set(next, last);

let onComplete = (last, delegate, exn) => {
  let exn =
    switch (exn) {
    | Some(_) => exn
    | None =>
      if (RxMutableOption.isEmpty(last)) {
        Some(RxEmptyException.Exn);
      } else {
        let lastValue = RxMutableOption.get(last);
        delegate |> RxSubscriber.next(lastValue);
        None;
      }
    };
  delegate |> RxSubscriber.complete(~exn?);
};

let create = subscriber => {
  let last = RxMutableOption.create();
  let disposable = RxDisposable.create1(RxMutableOption.unset, last);

  subscriber
  |> RxSubscriber.decorate1(~onNext, ~onComplete, last)
  |> RxSubscriber.addDisposable(disposable);
};
let operator = observer => {
  let switchObserver = ref(Observer.disposed);
  let innerSubscription = SerialDisposable.create();
  let latest = ref(0);
  let lock = Lock.create();

  let doSubscribeInner = (id, next) => {
    innerSubscription |> SerialDisposable.set(Disposable.disposed);
    let onNext = next => {
      lock |> Lock.acquire;
      if (latest^ === id) {
        observer |> Observer.next(next);
      };
      lock |> Lock.release;
    };

    let onComplete = exn =>
      switch (exn) {
      | Some(_) =>
        if (latest^ === id) {
          switchObserver^ |> Observer.complete(~exn?);
        }
      | None => ()
      };

    let newInnerSubscription =
      next |> ObservableSource.subscribeWith(~onNext, ~onComplete);
    innerSubscription
    |> SerialDisposable.set(
         newInnerSubscription |> CompositeDisposable.asDisposable,
       );
  };

  let onNext = next => {
    lock |> Lock.acquire;
    incr(latest);
    let id = latest^;
    lock |> Lock.release;

    doSubscribeInner(id, next);
  };

  let onComplete = exn => {
    lock |> Lock.acquire;
    innerSubscription |> SerialDisposable.dispose;
    observer |> Observer.complete(~exn?);
    lock |> Lock.release;
  };

  switchObserver :=
    observer
    |> Observer.delegate(~onNext, ~onComplete)
    |> Observer.addDisposable(
         SerialDisposable.asDisposable(innerSubscription),
       );
  switchObserver^;
};

let lift = observable => observable |> ObservableSource.lift(operator);
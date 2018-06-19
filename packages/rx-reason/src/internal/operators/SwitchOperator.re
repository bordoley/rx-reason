let operator = subscriber => {
  let switchSubscriber = ref(Subscriber.disposed);
  let innerSubscription = SerialDisposable.create();
  let latest = ref(0);
  let lock = Lock.create();

  let doSubscribeInner = (id, next) => {
    innerSubscription |> SerialDisposable.set(Disposable.disposed);
    let onNext = next => {
      lock |> Lock.acquire;
      if (latest^ === id) {
        subscriber |> Subscriber.next(next);
      };
      lock |> Lock.release;
    };

    let onComplete = exn =>
      switch (exn) {
      | Some(_) =>
        if (latest^ === id) {
          switchSubscriber^ |> Subscriber.complete(~exn?);
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
    subscriber |> Subscriber.complete(~exn?);
    lock |> Lock.release;
  };

  switchSubscriber :=
    subscriber
    |> Subscriber.delegate(~onNext, ~onComplete)
    |> Subscriber.addDisposable(
         SerialDisposable.asDisposable(innerSubscription),
       );
  switchSubscriber^;
};

let lift = observable => observable |> ObservableSource.lift(operator);
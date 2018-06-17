
let operator = observer => {
  let switchObserver = ref(Observer.disposed);
  let innerSubscription = SerialDisposable.create();
  let latest = ref(0);
  let lock = Lock.create();

  let doSubscribeInner = (id, next) => {
    innerSubscription |> SerialDisposable.set(Disposable.disposed);
    let newInnerSubscription =
      ObservableSource.subscribeWith(
        ~onNext=
          next => {
            lock |> Lock.acquire;
            if (latest^ === id) {
              observer |> Observer.next(next);
            };
            lock |> Lock.release;
          },
        ~onComplete=
          exn =>
            switch (exn) {
            | Some(_) =>
              if (latest^ === id) {
                switchObserver^ |> Observer.complete(~exn?);
              }
            | None => ()
            },
        next,
      );
    innerSubscription |> SerialDisposable.set(newInnerSubscription);
  };

  switchObserver :=
    Observer.create(
      ~onNext=
        next => {
          lock |> Lock.acquire;
          incr(latest);
          let id = latest^;
          lock |> Lock.release;

          doSubscribeInner(id, next);
        },
      ~onComplete=
        exn => {
          lock |> Lock.acquire;
          innerSubscription |> SerialDisposable.dispose;
          observer |> Observer.complete(~exn?);
          lock |> Lock.release;
        },
      ~onDispose=
        () => {
          innerSubscription |> SerialDisposable.dispose;
          observer |> Observer.dispose;
        },
    );
  switchObserver^;
};

let lift = observable =>
  observable |> ObservableSource.lift(operator);
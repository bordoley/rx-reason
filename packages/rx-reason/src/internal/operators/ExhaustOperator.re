let operator = observer => {
  let innerSubscription = SerialDisposable.create();
  let exhaustObserver = ref(Observer.disposed);

  let hasActiveSubscription = () =>
    innerSubscription |> SerialDisposable.get |> Disposable.isDisposed |> (!);

  let completeObserver = exn => {
    innerSubscription |> SerialDisposable.dispose;
    observer |> Observer.complete(~exn?);
  };

  exhaustObserver :=
    Observer.create(
      ~onNext=
        next => {
          let hasActiveSubscription = hasActiveSubscription();
          if (! hasActiveSubscription) {
            let subscription =
              ObservableSource.subscribeWith(
                ~onNext=Observer.forwardOnNext(observer),
                ~onComplete=
                  exn =>
                    if (exhaustObserver^ |> Observer.isStopped) {
                      completeObserver(exn);
                    } else if (exn !== None) {
                      exhaustObserver^ |> Observer.complete(~exn?);
                    },
                next,
              );
            innerSubscription |> SerialDisposable.set(subscription);
          };
        },
      ~onComplete=
        exn => {
          let hasActiveSubscription = hasActiveSubscription();
          switch (exn) {
          | Some(_)
          | None when ! hasActiveSubscription => completeObserver(exn)
          | _ => ()
          };
        },
      ~onDispose=
        () => {
          innerSubscription |> SerialDisposable.dispose;
          observer |> Observer.dispose;
        },
    );
  exhaustObserver^;
};

let lift = observable => observable |> ObservableSource.lift(operator);
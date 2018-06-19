let operator = subscriber => {
  let innerSubscription = SerialDisposable.create();
  let exhaustSubscriber = ref(Subscriber.disposed);

  let hasActiveSubscription = () =>
    innerSubscription |> SerialDisposable.get |> Disposable.isDisposed |> (!);

  let completeSubscriber = exn => {
    innerSubscription |> SerialDisposable.dispose;
    subscriber |> Subscriber.complete(~exn?);
  };

  let onNext = next => {
    let hasActiveSubscription = hasActiveSubscription();
    if (! hasActiveSubscription) {
      let subscription =
        ObservableSource.subscribeWith(
          ~onNext=Subscriber.forwardOnNext(subscriber),
          ~onComplete=
            exn =>
              if (exhaustSubscriber^ |> Subscriber.isStopped) {
                completeSubscriber(exn);
              } else if (exn !== None) {
                exhaustSubscriber^ |> Subscriber.complete(~exn?);
              },
          next,
        );
      innerSubscription
      |> SerialDisposable.set(
           subscription |> CompositeDisposable.asDisposable,
         );
    };
  };

  let onComplete = exn => {
    let hasActiveSubscription = hasActiveSubscription();
    switch (exn) {
    | Some(_)
    | None when ! hasActiveSubscription => completeSubscriber(exn)
    | _ => ()
    };
  };

  exhaustSubscriber :=
    subscriber
    |> Subscriber.delegate(~onNext, ~onComplete)
    |> Subscriber.addDisposable(
         innerSubscription |> SerialDisposable.asDisposable,
       );
  exhaustSubscriber^;
};

let lift = observable => observable |> ObservableSource.lift(operator);
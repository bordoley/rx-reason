let onNext = (_, delegate, v) => delegate |> RxSubscriber.next(v);

let rec onComplete = (ctx, delegate, exn) => {
  let (shouldRepeat, _, _) = ctx;
  let shouldComplete = !shouldRepeat(exn);

  shouldComplete ?
    delegate |> RxSubscriber.complete(~exn?) :
    setupSubscription(ctx, delegate);
}
and setupSubscription = (ctx, delegate) => {
  let (_, observable, subscription) = ctx;
  let alreadyDisposed = subscription |> RxSerialDisposable.isDisposed;

  if (!alreadyDisposed) {
    subscription
    |> RxSerialDisposable.getInnerDisposable
    |> RxDisposable.dispose;

    let newInnerSubscription =
      observable
      |> ObserveObservable.create2(~onNext, ~onComplete, ctx, delegate)
      |> RxObservable.connect;

    subscription
    |> RxSerialDisposable.setInnerDisposable(newInnerSubscription);
  };
};

let create = (shouldRepeat, observable) => {
  let onComplete =
    (. ctx, delegate, exn) => {
      let (shouldRepeat, _, _) = ctx;
      let shouldComplete = !shouldRepeat(exn);

      shouldComplete ?
        delegate |> RxSubscriber.complete(~exn?) :
        setupSubscription(ctx, delegate);
    };

  subscriber => {
    let subscription = RxSerialDisposable.create();
    let disposable = subscription |> RxSerialDisposable.asDisposable;
    let ctx = (shouldRepeat, observable, subscription);

    subscriber
    |> RxSubscriber.decorateOnComplete1(onComplete, ctx)
    |> RxSubscriber.addDisposable(disposable);
  };
};
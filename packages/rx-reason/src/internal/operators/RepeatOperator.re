type context('a) = {
  observable: ObservableSource.t('a),
  shouldRepeat: option(exn) => bool,
  subscription: SerialDisposable.t,
};

let operator = {
  let rec onComplete = {
    let impl = ({shouldRepeat} as ctx, delegate, exn) => {
      let shouldComplete =
        try (! shouldRepeat(exn)) {
        | exn =>
          delegate |> Subscriber.complete(~exn);
          Functions.returnUnit();
        };

      shouldComplete ?
        delegate |> Subscriber.complete(~exn?) :
        setupSubscription(ctx, delegate);
    };

    (ctx, delegate, exn) =>
      Functions.earlyReturnsUnit3(impl, ctx, delegate, exn);
  }
  and setupSubscription = ({observable, subscription} as ctx, delegate) => {
    let alreadyDisposed = subscription |> SerialDisposable.isDisposed;

    if (! alreadyDisposed) {
      subscription |> SerialDisposable.get |> Disposable.dispose;
      let newInnerSubscription =
        observable
        |> ObservableSource.subscribeWith(
             ~onNext=next => delegate |> Subscriber.next(next),
             ~onComplete=exn => onComplete(ctx, delegate, exn),
           );
      subscription
      |> SerialDisposable.set(
           newInnerSubscription |> CompositeDisposable.asDisposable,
         );
    };
  };

  (shouldRepeat, observable, subscriber) => {
    let context = {
      observable,
      shouldRepeat,
      subscription: SerialDisposable.create(),
    };
    subscriber
    |> Subscriber.delegate(
         ~onNext=Subscriber.forwardOnNext,
         ~onComplete,
         context,
       )
    |> Subscriber.addDisposable(
         SerialDisposable.asDisposable(context.subscription),
       );
  };
};

let lift = (shouldRepeat, observable) =>
  observable |> ObservableSource.lift(operator(shouldRepeat, observable));
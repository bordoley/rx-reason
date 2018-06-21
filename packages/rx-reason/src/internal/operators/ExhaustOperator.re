type context('a) = {
  innerSubscription: SerialDisposable.t,
  mutable self: Subscriber.t('a),
};

let operator = {
  let hasActiveSubscription = ({innerSubscription}) =>
    innerSubscription |> SerialDisposable.get |> Disposable.isDisposed |> (!);

  let completeSubscriber = ({innerSubscription}, delegate, exn) => {
    innerSubscription |> SerialDisposable.dispose;
    delegate |> Subscriber.complete(~exn?);
  };

  let onNext = ({innerSubscription} as ctx, delegate, next) => {
    let hasActiveSubscription = hasActiveSubscription(ctx);
    if (! hasActiveSubscription) {
      let subscription =
        ObservableSource.subscribeWith(
          ~onNext=next => delegate |> Subscriber.next(next),
          ~onComplete=
            exn =>
              if (ctx.self |> Subscriber.isStopped) {
                completeSubscriber(ctx, delegate, exn);
              } else if (exn !== None) {
                ctx.self |> Subscriber.complete(~exn?);
              },
          next,
        );
      innerSubscription
      |> SerialDisposable.set(
           subscription |> CompositeDisposable.asDisposable,
         );
    };
  };

  let onComplete = (ctx, delegate, exn) => {
    let hasActiveSubscription = hasActiveSubscription(ctx);
    switch (exn) {
    | Some(_)
    | None when ! hasActiveSubscription =>
      completeSubscriber(ctx, delegate, exn)
    | _ => ()
    };
  };

  subscriber => {
    let context = {
      innerSubscription: SerialDisposable.create(),
      self: Subscriber.disposed,
    };
    context.self =
      subscriber
      |> Subscriber.delegate(~onNext, ~onComplete, context)
      |> Subscriber.addDisposable(
           context.innerSubscription |> SerialDisposable.asDisposable,
         );
    context.self;
  };
};

let lift = observable => observable |> ObservableSource.lift(operator);
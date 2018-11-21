type context('a) = {
  innerSubscription: SerialDisposable.t,
  mutable self: Subscriber.t('a),
};

let operator = {
  let hasActiveSubscription = ({innerSubscription}) =>
    innerSubscription
    |> SerialDisposable.getInnerDisposable
    |> Disposable.isDisposed
    |> (!);

  let completeSubscriber = ({innerSubscription}, delegate, exn) => {
    innerSubscription |> SerialDisposable.dispose;
    delegate |> Subscriber.complete(~exn?);
  };

  let onNext = {
    let onComplete = (ctx, delegate, exn) =>
      if (ctx.self |> Subscriber.isStopped) {
        completeSubscriber(ctx, delegate, exn);
      } else if (exn !== None) {
        ctx.self |> Subscriber.complete(~exn?);
      };

    ({innerSubscription} as ctx, delegate, next) => {
      let hasActiveSubscription = hasActiveSubscription(ctx);
      if (! hasActiveSubscription) {
        let subscription =
          Subscriber.create2(
            ~onNext=Subscriber.forwardOnNext1,
            ~onComplete,
            ctx,
            delegate,
          );
        next |> Observable.subscribeWith(subscription);
        innerSubscription
        |> SerialDisposable.setInnerDisposable(
             subscription |> Subscriber.asDisposable,
           );
      };
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
      |> Subscriber.decorate1(~onNext, ~onComplete, context)
      |> Subscriber.addTeardown1(
           SerialDisposable.dispose,
           context.innerSubscription,
         );
    context.self;
  };
};
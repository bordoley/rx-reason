type context('a) = {
  innerSubscription: RxSerialDisposable.t,
  mutable self: RxSubscriber.t('a),
};

let operator = {
  let hasActiveSubscription = ({innerSubscription}) =>
    innerSubscription
    |> RxSerialDisposable.getInnerDisposable
    |> RxDisposable.isDisposed
    |> (!);

  let completeSubscriber = ({innerSubscription}, delegate, exn) => {
    innerSubscription |> RxSerialDisposable.dispose;
    delegate |> RxSubscriber.complete(~exn?);
  };

  let onNext = {
    let onComplete = (ctx, delegate, exn) =>
      if (ctx.self |> RxSubscriber.isStopped) {
        completeSubscriber(ctx, delegate, exn);
      } else if (exn !== None) {
        ctx.self |> RxSubscriber.complete(~exn?);
      };

    ({innerSubscription} as ctx, delegate, next) => {
      let hasActiveSubscription = hasActiveSubscription(ctx);
      if (! hasActiveSubscription) {
        let subscription =
          next
          |> RxObservable.observe2(
               ~onNext=RxSubscriber.forwardOnNext1,
               ~onComplete,
               ctx,
               delegate,
             )
          |> RxObservable.subscribe;
        innerSubscription
        |> RxSerialDisposable.setInnerDisposable(subscription);
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
      innerSubscription: RxSerialDisposable.create(),
      self: RxSubscriber.disposed,
    };

    context.self =
      subscriber
      |> RxSubscriber.decorate1(~onNext, ~onComplete, context)
      |> RxSubscriber.addTeardown1(
           RxSerialDisposable.dispose,
           context.innerSubscription,
         );
    context.self;
  };
};
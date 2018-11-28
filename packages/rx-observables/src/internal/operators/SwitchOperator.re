type context('a) = {
  innerSubscription: RxSerialDisposable.t,
  mutable latest: int,
  lock: RxLock.t,
  mutable self: RxSubscriber.t('a),
};

let operator = {
  let onNext = {
    let onNextInner = (id, ctx, delegate, next) => {
      ctx.lock |> RxLock.acquire;
      if (ctx.latest === id) {
        delegate |> RxSubscriber.next(next);
      };
      ctx.lock |> RxLock.release;
    };

    let onCompleteInner = (id, ctx, _, exn) =>
      switch (exn) {
      | Some(_) =>
        if (ctx.latest === id) {
          ctx.self |> RxSubscriber.complete(~exn?);
        }
      | None => ()
      };

    (ctx, delegate, next) => {
      ctx.lock |> RxLock.acquire;
      let id = ctx.latest + 1;
      ctx.latest = id;
      ctx.lock |> RxLock.release;

      ctx.innerSubscription
      |> RxSerialDisposable.setInnerDisposable(RxDisposable.disposed);

      let newInnerSubscription =
        next
        |> RxObservable.observe3(
             ~onNext=onNextInner,
             ~onComplete=onCompleteInner,
             id,
             ctx,
             delegate,
           )
        |> RxObservable.subscribe;

      ctx.innerSubscription
      |> RxSerialDisposable.setInnerDisposable(newInnerSubscription);
    };
  };

  let onComplete = (ctx, delegate, exn) => {
    ctx.lock |> RxLock.acquire;
    ctx.innerSubscription |> RxSerialDisposable.dispose;
    delegate |> RxSubscriber.complete(~exn?);
    ctx.lock |> RxLock.release;
  };

  subscriber => {
    let context = {
      innerSubscription: RxSerialDisposable.create(),
      latest: 0,
      lock: RxLock.create(),
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
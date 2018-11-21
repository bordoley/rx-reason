type context('a) = {
  innerSubscription: SerialDisposable.t,
  mutable latest: int,
  lock: Lock.t,
  mutable self: Subscriber.t('a),
};

let operator = {
  let onNext = {
    let onNextInner = (id, ctx, delegate, next) => {
      ctx.lock |> Lock.acquire;
      if (ctx.latest === id) {
        delegate |> Subscriber.next(next);
      };
      ctx.lock |> Lock.release;
    };

    let onCompleteInner = (id, ctx, _, exn) =>
      switch (exn) {
      | Some(_) =>
        if (ctx.latest === id) {
          ctx.self |> Subscriber.complete(~exn?);
        }
      | None => ()
      };

    (ctx, delegate, next) => {
      ctx.lock |> Lock.acquire;
      let id = ctx.latest + 1;
      ctx.latest = id;
      ctx.lock |> Lock.release;

      ctx.innerSubscription
      |> SerialDisposable.setInnerDisposable(Disposable.disposed);

      let newInnerSubscription =
        Subscriber.create3(
          ~onNext=onNextInner,
          ~onComplete=onCompleteInner,
          id,
          ctx,
          delegate,
        );

      next |> Observable.subscribeWith(newInnerSubscription);

      ctx.innerSubscription
      |> SerialDisposable.setInnerDisposable(
           newInnerSubscription |> Subscriber.asDisposable,
         );
    };
  };

  let onComplete = (ctx, delegate, exn) => {
    ctx.lock |> Lock.acquire;
    ctx.innerSubscription |> SerialDisposable.dispose;
    delegate |> Subscriber.complete(~exn?);
    ctx.lock |> Lock.release;
  };

  subscriber => {
    let context = {
      innerSubscription: SerialDisposable.create(),
      latest: 0,
      lock: Lock.create(),
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
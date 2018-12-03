type context('a) = {
  innerSubscription: RxSerialDisposable.t,
  mutable latest: int,
  lock: RxLock.t,
  mutable self: RxSubscriber.t('a),
};

module InnerOperator = {
  let onNext = (id, ctx, delegate, _, next) => {
    ctx.lock |> RxLock.acquire;
    if (ctx.latest === id) {
      delegate |> RxSubscriber.next(next);
    };
    ctx.lock |> RxLock.release;
  };

  let onComplete = (id, ctx, _, _, exn) =>
    switch (exn) {
    | Some(_) =>
      if (ctx.latest === id) {
        ctx.self |> RxSubscriber.complete(~exn?);
      }
    | None => ()
    };

  let create = (id, ctx, delegate) =>
    RxSubscriber.decorate3(~onNext, ~onComplete, id, ctx, delegate);
};

let onNext = (ctx, delegate, next) => {
  ctx.lock |> RxLock.acquire;
  let id = ctx.latest + 1;
  ctx.latest = id;
  ctx.lock |> RxLock.release;

  ctx.innerSubscription
  |> RxSerialDisposable.setInnerDisposable(RxDisposable.disposed);

  let newInnerSubscription =
    next
    |> RxObservable.lift(InnerOperator.create(id, ctx, delegate))
    |> RxObservable.subscribe;

  ctx.innerSubscription
  |> RxSerialDisposable.setInnerDisposable(newInnerSubscription);
};

let onComplete = (ctx, delegate, exn) => {
  ctx.lock |> RxLock.acquire;
  ctx.innerSubscription |> RxSerialDisposable.dispose;
  delegate |> RxSubscriber.complete(~exn?);
  ctx.lock |> RxLock.release;
};

let create = subscriber => {
  let context = {
    innerSubscription: RxSerialDisposable.create(),
    latest: 0,
    lock: RxLock.create(),
    self: RxSubscriber.disposed,
  };

  let innerDisposable =
    context.innerSubscription |> RxSerialDisposable.asDisposable;

  context.self =
    subscriber
    |> RxSubscriber.decorate1(~onNext, ~onComplete, context)
    |> RxSubscriber.addDisposable(innerDisposable);
  context.self;
};
type context('a) = {
  innerSubscription: SerialDisposable.t,
  latest: ref(int),
  lock: Lock.t,
  mutable self: Subscriber.t('a),
};

let operator = {
  let doSubscribeInner = {
    let onNext = (id, ctx, delegate, next) => {
      ctx.lock |> Lock.acquire;
      if (ctx.latest^ === id) {
        delegate |> Subscriber.next(next);
      };
      ctx.lock |> Lock.release;
    };

    let onComplete = (id, ctx, _, exn) =>
      switch (exn) {
      | Some(_) =>
        if (ctx.latest^ === id) {
          ctx.self |> Subscriber.complete(~exn?);
        }
      | None => ()
      };

    (id, {innerSubscription} as ctx, delegate, next) => {
      innerSubscription |> SerialDisposable.set(Disposable.disposed);

      let newInnerSubscription =
        ObservableSource.subscribeWith3(
          ~onNext,
          ~onComplete,
          id,
          ctx,
          delegate,
          next,
        );

      innerSubscription
      |> SerialDisposable.set(
           newInnerSubscription |> CompositeDisposable.asDisposable,
         );
    };
  };

  let onNext = ({latest, lock} as ctx, delegate, next) => {
    lock |> Lock.acquire;
    incr(latest);
    let id = latest^;
    lock |> Lock.release;

    doSubscribeInner(id, ctx, delegate, next);
  };

  let onComplete = ({innerSubscription, lock}, delegate, exn) => {
    lock |> Lock.acquire;
    innerSubscription |> SerialDisposable.dispose;
    delegate |> Subscriber.complete(~exn?);
    lock |> Lock.release;
  };

  subscriber => {
    let context = {
      innerSubscription: SerialDisposable.create(),
      latest: ref(0),
      lock: Lock.create(),
      self: Subscriber.disposed,
    };

    context.self =
      subscriber
      |> Subscriber.delegate1(~onNext, ~onComplete, context)
      |> Subscriber.addSerialDisposable(context.innerSubscription);
    context.self;
  };
};

let lift = observable => observable |> ObservableSource.lift(operator);
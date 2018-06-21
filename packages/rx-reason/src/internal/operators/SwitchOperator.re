type context('a) = {
  innerSubscription: SerialDisposable.t,
  latest: ref(int),
  lock: Lock.t,
  mutable self: Subscriber.t('a),
};

let operator = {
  let doSubscribeInner =
      (id, {innerSubscription, lock, latest} as ctx, delegate, next) => {
    innerSubscription |> SerialDisposable.set(Disposable.disposed);

    let newInnerSubscription = {
      let onNext = next => {
        lock |> Lock.acquire;
        if (latest^ === id) {
          delegate |> Subscriber.next(next);
        };
        lock |> Lock.release;
      };

      let onComplete = exn =>
        switch (exn) {
        | Some(_) =>
          if (latest^ === id) {
            ctx.self |> Subscriber.complete(~exn?);
          }
        | None => ()
        };

      next |> ObservableSource.subscribeWith(~onNext, ~onComplete);
    };

    innerSubscription
    |> SerialDisposable.set(
         newInnerSubscription |> CompositeDisposable.asDisposable,
       );
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
      |> Subscriber.addDisposable(
           SerialDisposable.asDisposable(context.innerSubscription),
         );
    context.self;
  };
};


let lift = (observable) =>
  observable |> ObservableSource.lift(operator);
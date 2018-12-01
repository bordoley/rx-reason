type context('a) = {
  mutable completed: bool,
  innerSubscription: RxSerialDisposable.t,
  mutable self: RxSubscriber.t('a),
};

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
    if (ctx.completed) {
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
             ~onNext=SubscriberForward.onNext1,
             ~onComplete,
             ctx,
             delegate,
           )
        |> RxObservable.subscribe;
      innerSubscription |> RxSerialDisposable.setInnerDisposable(subscription);
    };
  };
};

let onComplete = (ctx, delegate, exn) => {
  ctx.completed = true;
  let hasActiveSubscription = hasActiveSubscription(ctx);
  switch (exn) {
  | Some(_)
  | None when ! hasActiveSubscription =>
    completeSubscriber(ctx, delegate, exn)
  | _ => ()
  };
};

let operator = subscriber => {
  let context = {
    completed: false,
    innerSubscription: RxSerialDisposable.create(),
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
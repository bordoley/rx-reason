type context('a, 'b, 'c) = {
  value0: RxMutableOption.t('a),
  value1: RxMutableOption.t('b),
  selector: ('a, 'b) => 'c,
  subscription0: ref(RxDisposable.t),
  subscription1: ref(RxDisposable.t),
  subscriber: RxSubscriber.t('c),
};

let onNext = (ctx, value, _, next) => {
  value |> RxMutableOption.set(next);

  let haveValues =
    RxMutableOption.isNotEmpty(ctx.value0)
    && RxMutableOption.isNotEmpty(ctx.value1);
  if (haveValues) {
    let next =
      ctx.selector(
        RxMutableOption.get(ctx.value0),
        RxMutableOption.get(ctx.value1),
      );
    ctx.subscriber |> RxSubscriber.next(next);
  };
};

let onComplete = (ctx, _, other, exn) =>
  switch (exn) {
  | Some(_) => ctx.subscriber |> RxSubscriber.complete(~exn?)
  | None =>
    let shouldComplete = RxDisposable.isDisposed(other^);
    if (shouldComplete) {
      ctx.subscriber |> RxSubscriber.complete(~exn?);
    };
  };

let observableSource = (selector, observable0, observable1, subscriber) => {
  let ctx = {
    value0: RxMutableOption.create(),
    value1: RxMutableOption.create(),
    selector,
    subscription0: ref(RxDisposable.disposed),
    subscription1: ref(RxDisposable.disposed),
    subscriber,
  };

  ctx.subscription0 :=
    observable0
    |> RxObservable.observe3(
         ~onNext,
         ~onComplete,
         ctx,
         ctx.value0,
         ctx.subscription1,
       )
    |> RxObservable.subscribe;

  ctx.subscription1 :=
    observable1
    |> RxObservable.observe3(
         ~onNext,
         ~onComplete,
         ctx,
         ctx.value1,
         ctx.subscription0,
       )
    |> RxObservable.subscribe;

  subscriber
  |> RxSubscriber.addDisposable(ctx.subscription0^)
  |> RxSubscriber.addDisposable(ctx.subscription1^)
  |> RxSubscriber.addDisposable(
       RxDisposable.create1(RxMutableOption.unset, ctx.value0),
     )
  |> RxSubscriber.addDisposable(
       RxDisposable.create1(RxMutableOption.unset, ctx.value1),
     )
  |> ignore;
};

let create = (~selector, observable0, observable1) =>
  RxObservable.create3(observableSource, selector, observable0, observable1);
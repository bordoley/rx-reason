type t('a) =
  | Disposed
  | AutoDisposing(RxCompositeDisposable.t)
  | Decorating(
      t('b),
      RxAtomic.t(bool),
      (t('b), 'a) => unit,
      (t('b), option(exn)) => unit,
    )
    : t('a)
  | Decorating1(
      t('b),
      RxAtomic.t(bool),
      'ctx0,
      ('ctx0, t('b), 'a) => unit,
      ('ctx0, t('b), option(exn)) => unit,
    )
    : t('a)
  | Decorating2(
      t('b),
      RxAtomic.t(bool),
      'ctx0,
      'ctx1,
      ('ctx0, 'ctx1, t('b), 'a) => unit,
      ('ctx0, 'ctx1, t('b), option(exn)) => unit,
    )
    : t('a)
  | Decorating3(
      t('b),
      RxAtomic.t(bool),
      'ctx0,
      'ctx1,
      'ctx2,
      ('ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
      ('ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
    )
    : t('a)
  | Decorating4(
      t('b),
      RxAtomic.t(bool),
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), 'a) => unit,
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), option(exn)) => unit,
    )
    : t('a)
  | Decorating5(
      t('b),
      RxAtomic.t(bool),
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), 'a) => unit,
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), option(exn)) => unit,
    )
    : t('a);

let rec asCompositeDisposable: type a. t(a) => RxCompositeDisposable.t =
  fun
  | Disposed => RxCompositeDisposable.disposed
  | AutoDisposing(disposable) => disposable
  | Decorating(delegate, _, _, _) => delegate |> asCompositeDisposable
  | Decorating1(delegate, _, _, _, _) => delegate |> asCompositeDisposable
  | Decorating2(delegate, _, _, _, _, _) => delegate |> asCompositeDisposable
  | Decorating3(delegate, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating4(delegate, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating5(delegate, _, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable;

let addDisposable = (disposable, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.addDisposable(disposable)
  |> ignore;
  subscriber;
};

let removeDisposable = (disposable, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> RxCompositeDisposable.removeDisposable(disposable)
  |> ignore;
  subscriber;
};

let asDisposable = subscriber =>
  subscriber |> asCompositeDisposable |> RxCompositeDisposable.asDisposable;

let dispose = subscriber =>
  subscriber |> asCompositeDisposable |> RxCompositeDisposable.dispose;

let isDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> RxCompositeDisposable.isDisposed;

let isStopped =
  fun
  | Disposed => true
  | AutoDisposing(disposable) => RxCompositeDisposable.isDisposed(disposable)
  | Decorating(_, isStopped, _, _)
  | Decorating1(_, isStopped, _, _, _)
  | Decorating2(_, isStopped, _, _, _, _)
  | Decorating3(_, isStopped, _, _, _, _, _)
  | Decorating4(_, isStopped, _, _, _, _, _, _)
  | Decorating5(_, isStopped, _, _, _, _, _, _, _) =>
    RxAtomic.get(isStopped);

let rec completeWithResult: 'a. (~exn: exn=?, t('a)) => bool = {
  let shouldComplete =
    fun
    | Disposed => false
    | AutoDisposing(disposable) =>
      !RxCompositeDisposable.isDisposed(disposable)
    | Decorating(_, isStopped, _, _)
    | Decorating1(_, isStopped, _, _, _)
    | Decorating2(_, isStopped, _, _, _, _)
    | Decorating3(_, isStopped, _, _, _, _, _)
    | Decorating4(_, isStopped, _, _, _, _, _, _)
    | Decorating5(_, isStopped, _, _, _, _, _, _, _) =>
      !RxAtomic.exchange(isStopped, true);

  let doComplete = (exn, subscriber) =>
    switch (subscriber) {
    | Disposed => ()
    | AutoDisposing(disposable) => disposable |> RxCompositeDisposable.dispose
    | Decorating(delegate, _, _, onComplete) => onComplete(delegate, exn)
    | Decorating1(delegate, _, ctx0, _, onComplete) =>
      onComplete(ctx0, delegate, exn)
    | Decorating2(delegate, _, ctx0, ctx1, _, onComplete) =>
      onComplete(ctx0, ctx1, delegate, exn)
    | Decorating3(delegate, _, ctx0, ctx1, ctx2, _, onComplete) =>
      onComplete(ctx0, ctx1, ctx2, delegate, exn)
    | Decorating4(delegate, _, ctx0, ctx1, ctx2, ctx3, _, onComplete) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, delegate, exn)
    | Decorating5(delegate, _, ctx0, ctx1, ctx2, ctx3, ctx4, _, onComplete) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, delegate, exn)
    };

  let completeDelegate = (exn, subscriber) =>
    switch (subscriber) {
    | Decorating(delegate, _, _, _) => delegate |> complete(~exn?)
    | Decorating1(delegate, _, _, _, _) => delegate |> complete(~exn?)
    | Decorating2(delegate, _, _, _, _, _) => delegate |> complete(~exn?)
    | Decorating3(delegate, _, _, _, _, _, _) => delegate |> complete(~exn?)
    | Decorating4(delegate, _, _, _, _, _, _, _) =>
      delegate |> complete(~exn?)
    | Decorating5(delegate, _, _, _, _, _, _, _, _) =>
      delegate |> complete(~exn?)
    | Disposed
    | AutoDisposing(_) => ()
    };

  (~exn=?, subscriber) => {
    let shouldComplete = shouldComplete(subscriber);
    if (shouldComplete) {
      try (doComplete(exn, subscriber)) {
      | exn => subscriber |> completeDelegate(Some(exn))
      };
    };
    shouldComplete;
  };
}
and complete: 'a. (~exn: exn=?, t('a)) => unit =
  (~exn=?, subscriber) => subscriber |> completeWithResult(~exn?) |> ignore;

let next = {
  let doNext = (next, subscriber) =>
    switch (subscriber) {
    | Decorating(delegate, _, onNext, _) => onNext(delegate, next)
    | Decorating1(delegate, _, ctx0, onNext, _) =>
      onNext(ctx0, delegate, next)
    | Decorating2(delegate, _, ctx0, ctx1, onNext, _) =>
      onNext(ctx0, ctx1, delegate, next)
    | Decorating3(delegate, _, ctx0, ctx1, ctx2, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, delegate, next)
    | Decorating4(delegate, _, ctx0, ctx1, ctx2, ctx3, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, ctx3, delegate, next)
    | Decorating5(delegate, _, ctx0, ctx1, ctx2, ctx3, ctx4, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, ctx3, ctx4, delegate, next)
    | Disposed
    | AutoDisposing(_) => ()
    };

  (next, subscriber) => {
    let isStopped = subscriber |> isStopped;
    if (!isStopped) {
      try (doNext(next, subscriber)) {
      | exn => subscriber |> complete(~exn)
      };
    };
  };
};

let notify = {
  let onComplete = (subscriber, exn) => subscriber |> complete(~exn?);
  let onNext = (subscriber, v) => subscriber |> next(v);

  (notif, subscriber) =>
    RxNotification.map1(~onNext, ~onComplete, subscriber, notif);
};

let create = () => {
  let disposable = RxCompositeDisposable.create();
  AutoDisposing(disposable);
};

let decorate = (~onNext, ~onComplete, subscriber) => {
  let isStopped = RxAtomic.make(false);
  let disposable = RxDisposable.create1(RxAtomic.setTrue, isStopped);
  subscriber |> addDisposable(disposable) |> ignore;
  Decorating(subscriber, isStopped, onNext, onComplete);
};

let decorateOnNext = {
  let onComplete = (subscriber, exn) => subscriber |> complete(~exn?);
  (onNext, subscriber) => decorate(~onNext, ~onComplete, subscriber);
};

let decorateOnComplete = {
  let onNext = (subscriber, v) => subscriber |> next(v);
  (onComplete, subscriber) => decorate(~onNext, ~onComplete, subscriber);
};

let decorate1 = (~onNext, ~onComplete, ctx0, subscriber) => {
  let isStopped = RxAtomic.make(false);
  let disposable = RxDisposable.create1(RxAtomic.setTrue, isStopped);
  subscriber |> addDisposable(disposable) |> ignore;
  Decorating1(subscriber, isStopped, ctx0, onNext, onComplete);
};

let decorateOnNext1 = {
  let onComplete = (_, subscriber, exn) => subscriber |> complete(~exn?);
  (onNext, subscriber) => decorate1(~onNext, ~onComplete, subscriber);
};

let decorateOnComplete1 = {
  let onNext = (_, subscriber, v) => subscriber |> next(v);
  (onComplete, subscriber) => decorate1(~onNext, ~onComplete, subscriber);
};

let decorate2 = (~onNext, ~onComplete, ctx0, ctx1, subscriber) => {
  let isStopped = RxAtomic.make(false);
  let disposable = RxDisposable.create1(RxAtomic.setTrue, isStopped);
  subscriber |> addDisposable(disposable) |> ignore;
  Decorating2(subscriber, isStopped, ctx0, ctx1, onNext, onComplete);
};

let decorateOnNext2 = {
  let onComplete = (_, _, subscriber, exn) => subscriber |> complete(~exn?);
  (onNext, subscriber) => decorate2(~onNext, ~onComplete, subscriber);
};

let decorateOnComplete2 = {
  let onNext = (_, _, subscriber, v) => subscriber |> next(v);
  (onComplete, subscriber) => decorate2(~onNext, ~onComplete, subscriber);
};

let decorate3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, subscriber) => {
  let isStopped = RxAtomic.make(false);
  let disposable = RxDisposable.create1(RxAtomic.setTrue, isStopped);
  subscriber |> addDisposable(disposable) |> ignore;
  Decorating3(subscriber, isStopped, ctx0, ctx1, ctx2, onNext, onComplete);
};

let decorateOnNext3 = {
  let onComplete = (_, _, _, subscriber, exn) =>
    subscriber |> complete(~exn?);
  (onNext, subscriber) => decorate3(~onNext, ~onComplete, subscriber);
};

let decorateOnComplete3 = {
  let onNext = (_, _, _, subscriber, v) => subscriber |> next(v);
  (onComplete, subscriber) => decorate3(~onNext, ~onComplete, subscriber);
};

let decorate4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, subscriber) => {
  let isStopped = RxAtomic.make(false);
  let disposable = RxDisposable.create1(RxAtomic.setTrue, isStopped);
  subscriber |> addDisposable(disposable) |> ignore;
  Decorating4(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    onNext,
    onComplete,
  );
};

let decorateOnNext4 = {
  let onComplete = (_, _, _, _, subscriber, exn) =>
    subscriber |> complete(~exn?);
  (onNext, subscriber) => decorate4(~onNext, ~onComplete, subscriber);
};

let decorateOnComplete4 = {
  let onNext = (_, _, _, _, subscriber, v) => subscriber |> next(v);
  (onComplete, subscriber) => decorate4(~onNext, ~onComplete, subscriber);
};

let decorate5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, subscriber) => {
  let isStopped = RxAtomic.make(false);
  let disposable = RxDisposable.create1(RxAtomic.setTrue, isStopped);
  subscriber |> addDisposable(disposable) |> ignore;
  Decorating5(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    onNext,
    onComplete,
  );
};

let decorateOnNext5 = {
  let onComplete = (_, _, _, _, _, subscriber, exn) =>
    subscriber |> complete(~exn?);
  (onNext, subscriber) => decorate5(~onNext, ~onComplete, subscriber);
};

let decorateOnComplete5 = {
  let onNext = (_, _, _, _, _, subscriber, v) => subscriber |> next(v);
  (onComplete, subscriber) => decorate5(~onNext, ~onComplete, subscriber);
};

let disposed = Disposed;
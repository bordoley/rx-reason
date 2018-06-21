type t('a) =
  | AutoDisposing(
      'a => unit,
      option(exn) => unit,
      ref(bool),
      CompositeDisposable.t,
    )
  | Delegating0(
                 (t('b), 'a) => unit,
                 (t('b), option(exn)) => unit,
                 ref(bool),
                 t('b),
               ): t('a)
  | Delegating1(
                 'ctx0,
                 ('ctx0, t('b), 'a) => unit,
                 ('ctx0, t('b), option(exn)) => unit,
                 ref(bool),
                 t('b),
               ): t('a)
  | Delegating2(
                 'ctx0,
                 'ctx1,
                 ('ctx0, 'ctx1, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, t('b), option(exn)) => unit,
                 ref(bool),
                 t('b),
               ): t('a)
  | Delegating3(
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 ('ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
                 ref(bool),
                 t('b),
               ): t('a)
  | Disposed;

let rec asCompositeDisposable: type a. t(a) => CompositeDisposable.t =
  fun
  | AutoDisposing(_, _, _, disposable) => disposable
  | Delegating0(_, _, _, delegate) => delegate |> asCompositeDisposable
  | Delegating1(_, _, _, _, delegate) => delegate |> asCompositeDisposable
  | Delegating2(_, _, _, _, _, delegate) => delegate |> asCompositeDisposable
  | Delegating3(_, _, _, _, _, _, delegate) =>
    delegate |> asCompositeDisposable
  | Disposed => CompositeDisposable.disposed;

let addDisposable = (disposable, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addDisposable(disposable)
  |> ignore;
  subscriber;
};

let addTeardown = (teardown, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown(teardown)
  |> ignore;
  subscriber;
};

let asDisposable = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.asDisposable;

let createAutoDisposing = (~onNext, ~onComplete) => {
  let isStopped = ref(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown(() => Volatile.write(true, isStopped))
  |> ignore;
  AutoDisposing(onNext, onComplete, isStopped, disposable);
};

let delegate = (~onNext, ~onComplete, subscriber) => {
  let stopped = ref(false);
  subscriber |> addTeardown(() => Volatile.write(true, stopped)) |> ignore;
  Delegating0(onNext, onComplete, stopped, subscriber);
};

let delegate1 = (~onNext, ~onComplete, ctx0, subscriber) => {
  let stopped = ref(false);
  subscriber |> addTeardown(() => Volatile.write(true, stopped)) |> ignore;
  Delegating1(ctx0, onNext, onComplete, stopped, subscriber);
};

let delegate2 = (~onNext, ~onComplete, ctx0, ctx1, subscriber) => {
  let stopped = ref(false);
  subscriber |> addTeardown(() => Volatile.write(true, stopped)) |> ignore;
  Delegating2(ctx0, ctx1, onNext, onComplete, stopped, subscriber);
};

let delegate3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, subscriber) => {
  let stopped = ref(false);
  subscriber |> addTeardown(() => Volatile.write(true, stopped)) |> ignore;
  Delegating3(ctx0, ctx1, ctx2, onNext, onComplete, stopped, subscriber);
};

let dispose = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.dispose;

let disposed = Disposed;

let isDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.isDisposed;

let isStopped =
  fun
  | AutoDisposing(_, _, stopped, _)
  | Delegating0(_, _, stopped, _) => Volatile.read(stopped)
  | Delegating1(_, _, _, stopped, _) => Volatile.read(stopped)
  | Delegating2(_, _, _, _, stopped, _) => Volatile.read(stopped)
  | Delegating3(_, _, _, _, _, stopped, _) => Volatile.read(stopped)
  | Disposed => true;

let shouldComplete =
  fun
  | AutoDisposing(_, _, stopped, _)
  | Delegating0(_, _, stopped, _)
  | Delegating1(_, _, _, stopped, _)
  | Delegating2(_, _, _, _, stopped, _)
  | Delegating3(_, _, _, _, _, stopped, _) =>
    ! Interlocked.exchange(true, stopped)
  | Disposed => false;

let completeWithResult = {
  let doComplete = (exn, subscriber) =>
    switch (subscriber) {
    | AutoDisposing(_, onComplete, _, disposable) =>
      try (onComplete(exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | Delegating0(_, onComplete, _, delegate) => onComplete(delegate, exn)
    | Delegating1(ctx0, _, onComplete, _, delegate) =>
      onComplete(ctx0, delegate, exn)
    | Delegating2(ctx0, ctx1, _, onComplete, _, delegate) =>
      onComplete(ctx0, ctx1, delegate, exn)
    | Delegating3(ctx0, ctx1, ctx2, _, onComplete, _, delegate) =>
      onComplete(ctx0, ctx1, ctx2, delegate, exn)
    | Disposed => ()
    };

  (~exn=?, subscriber) => {
    let shouldComplete = shouldComplete(subscriber);
    if (shouldComplete) {
      doComplete(exn, subscriber);
    };
    shouldComplete;
  };
};

let complete = (~exn=?, subscriber) =>
  subscriber |> completeWithResult(~exn?) |> ignore;

let next = {
  let doNext = (next, subscriber) =>
    switch (subscriber) {
    | AutoDisposing(onNext, _, _, disposable) =>
      try (onNext(next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | Delegating0(onNext, _, _, delegate) => onNext(delegate, next)
    | Delegating1(ctx0, onNext, _, _, delegate) =>
      onNext(ctx0, delegate, next)
    | Delegating2(ctx0, ctx1, onNext, _, _, delegate) =>
      onNext(ctx0, ctx1, delegate, next)
    | Delegating3(ctx0, ctx1, ctx2, onNext, _, _, delegate) =>
      onNext(ctx0, ctx1, ctx2, delegate, next)
    | Disposed => ()
    };

  (next, subscriber) => {
    let isStopped = subscriber |> isStopped;
    if (! isStopped) {
      doNext(next, subscriber);
    };
  };
};

let delegateOnComplete = (subscriber, exn) =>
  subscriber |> complete(~exn?);

let delegateOnComplete1 = (_, subscriber, exn) =>
  subscriber |> complete(~exn?);

let delegateOnComplete2 = (_, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let delegateOnComplete3 = (_, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let delegateOnNext = (subscriber, v) => subscriber |> next(v);

let delegateOnNext1 = (_, subscriber, v) => subscriber |> next(v);

let delegateOnNext2 = (_, _, subscriber, v) => subscriber |> next(v);

let delegateOnNext3 = (_, _, _, subscriber, v) => subscriber |> next(v);

let notify = (notif, subscriber) =>
  switch (notif) {
  | Notification.Next(v) => subscriber |> next(v)
  | Notification.Complete => subscriber |> complete
  | Notification.CompleteWithException(exn) => subscriber |> complete(~exn)
  };

let raiseIfDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.raiseIfDisposed;
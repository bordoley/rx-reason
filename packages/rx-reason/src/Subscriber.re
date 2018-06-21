type t('a) =
  | AutoDisposing(
      /** onNext */ ('a => unit),
      /** onComplete */ (option(exn) => unit),
      /** stopped */ ref(bool),
      /** disposable */ CompositeDisposable.t,
    )
  | Delegating(
                /** context */ 'ctx,
                /** onNext */ (('ctx, t('b), 'a) => unit),
                /** onComplete */ (('ctx, t('b), option(exn)) => unit),
                /** stopped */ ref(bool),
                /** delegate */ t('b),
              ): t('a)
  | Disposed;

let rec asCompositeDisposable: type a. t(a) => CompositeDisposable.t =
  fun
  | AutoDisposing(_, _, _, disposable) => disposable
  | Delegating(_, _, _, _, delegate) => delegate |> asCompositeDisposable
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

let delegate = (~onNext, ~onComplete, context, subscriber) => {
  /**
   * Ideally the delegating constructor would only require a single reference
   * to the subscriber, but the type system won't allow calling
   * asCompositeDisposable recursively on the enclosed subscriber in this case.
   */
  let stopped = ref(false);
  subscriber |> addTeardown(() => Volatile.write(true, stopped)) |> ignore;
  Delegating(context, onNext, onComplete, stopped, subscriber);
};

let dispose = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.dispose;

let disposed = Disposed;

let isDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.isDisposed;

let isStopped =
  fun
  | AutoDisposing(_, _, stopped, _)
  | Delegating(_, _, _, stopped, _) => Volatile.read(stopped)
  | Disposed => true;

let completeWithResult = (~exn=?, subscriber) =>
  switch (subscriber) {
  | AutoDisposing(_, onComplete, stopped, disposable) =>
    let shouldComplete = ! Interlocked.exchange(true, stopped);
    if (shouldComplete) {
      try (onComplete(exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
    };
    disposable |> CompositeDisposable.dispose;
    shouldComplete;
  | Delegating(ctx, _, onComplete, stopped, delegate) =>
    let shouldComplete = ! Interlocked.exchange(true, stopped);
    if (shouldComplete) {
      onComplete(ctx, delegate, exn);
    };
    shouldComplete;
  | Disposed => false
  };

let complete = (~exn=?, subscriber) =>
  subscriber |> completeWithResult(~exn?) |> ignore;

let next = (next, subscriber) =>
  switch (subscriber) {
  | AutoDisposing(onNext, _, _, _) =>
    let isStopped = subscriber |> isStopped;
    if (! isStopped) {
      try (onNext(next)) {
      | exn =>
        subscriber |> dispose;
        raise(exn);
      };
    };
  | Delegating(ctx, onNext, _, _, delegate) =>
    let isStopped = subscriber |> isStopped;
    if (! isStopped) {
      onNext(ctx, delegate, next);
    };
  | Disposed => ()
  };

let forwardOnComplete = (_, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnNext = (_, subscriber, v) => subscriber |> next(v);

let notify = (notif, subscriber) =>
  switch (notif) {
  | Notification.Next(v) => subscriber |> next(v)
  | Notification.Complete => subscriber |> complete
  | Notification.CompleteWithException(exn) => subscriber |> complete(~exn)
  };

let raiseIfDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.raiseIfDisposed;
type work('state) = (t('state), 'state) => unit
and work1('state, 'ctx0) = ('ctx0, t('state), 'state) => unit
and work2('state, 'ctx0, 'ctx1) = ('ctx0, 'ctx1, t('state), 'state) => unit
and work3('state, 'ctx0, 'ctx1, 'ctx2) =
  ('ctx0, 'ctx1, 'ctx2, t('state), 'state) => unit
and work4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('state), 'state) => unit
and work5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('state), 'state) => unit
and work6('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('state), 'state) => unit
and work7('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, t('state), 'state) => unit
and executor('state) =
  (~delay: float, t('state), 'state, ('state, t('state)) => unit) => unit
and t('state) =
  | C0(executor('state), SerialDisposable.t, work('state))
  | C1(executor('state), SerialDisposable.t, work1('state, 'ctx0), 'ctx0): t(
                                                                    'state,
                                                                    )
  | C2(
        executor('state),
        SerialDisposable.t,
        work2('state, 'ctx0, 'ctx1),
        'ctx0,
        'ctx1,
      ): t('state)
  | C3(
        executor('state),
        SerialDisposable.t,
        work3('state, 'ctx0, 'ctx1, 'ctx2),
        'ctx0,
        'ctx1,
        'ctx2,
      ): t('state)
  | C4(
        executor('state),
        SerialDisposable.t,
        work4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
      ): t('state)
  | C5(
        executor('state),
        SerialDisposable.t,
        work5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
      ): t('state)
  | C6(
        executor('state),
        SerialDisposable.t,
        work6('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
        'ctx5,
      ): t('state)
  | C7(
        executor('state),
        SerialDisposable.t,
        work7('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
        'ctx5,
        'ctx6,
      ): t('state)
  | Disposed;

type schedulerContinuation('state) = t('state);

module Work = {
  type t('state) = work('state);
  type t1('state, 'ctx0) = work1('state, 'ctx0);
  type t2('state, 'ctx0, 'ctx1) = work2('state, 'ctx0, 'ctx1);
  type t3('state, 'ctx0, 'ctx1, 'ctx2) = work3('state, 'ctx0, 'ctx1, 'ctx2);
  type t4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3) =
    work4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3);
  type t5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4) =
    work5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4);
  type t6('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5) =
    work6('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5);
  type t7('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6) =
    work7('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6);
};

module Executor = {
  type t('state) = executor('state);
};

let asSerialDisposable =
  fun
  | C0(_, disposable, _)
  | C1(_, disposable, _, _)
  | C2(_, disposable, _, _, _)
  | C3(_, disposable, _, _, _, _)
  | C4(_, disposable, _, _, _, _, _)
  | C5(_, disposable, _, _, _, _, _, _)
  | C6(_, disposable, _, _, _, _, _, _, _)
  | C7(_, disposable, _, _, _, _, _, _, _, _) => disposable
  | Disposed => SerialDisposable.disposed;

let asDisposable = continuation =>
  continuation |> asSerialDisposable |> SerialDisposable.asDisposable;

let get = continuation =>
  continuation |> asSerialDisposable |> SerialDisposable.get;

let set = (disposable, continuation) =>
  continuation |> asSerialDisposable |> SerialDisposable.set(disposable);

let disposed = Disposed;

let create = (executor, work) =>
  C0(executor, SerialDisposable.create(), work);

let create1 = (executor, work, ctx0) =>
  C1(executor, SerialDisposable.create(), work, ctx0);

let create2 = (executor, work, ctx0, ctx1) =>
  C2(executor, SerialDisposable.create(), work, ctx0, ctx1);

let create3 = (executor, work, ctx0, ctx1, ctx2) =>
  C3(executor, SerialDisposable.create(), work, ctx0, ctx1, ctx2);

let create4 = (executor, work, ctx0, ctx1, ctx2, ctx3) =>
  C4(executor, SerialDisposable.create(), work, ctx0, ctx1, ctx2, ctx3);

let create5 = (executor, work, ctx0, ctx1, ctx2, ctx3, ctx4) =>
  C5(executor, SerialDisposable.create(), work, ctx0, ctx1, ctx2, ctx3, ctx4);

let create6 = (executor, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) =>
  C6(
    executor,
    SerialDisposable.create(),
    work,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
  );

let create7 = (executor, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6) =>
  C7(
    executor,
    SerialDisposable.create(),
    work,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
  );

let dispose = continuation =>
  continuation |> asSerialDisposable |> SerialDisposable.dispose;

let isDisposed = continuation =>
  continuation |> asSerialDisposable |> SerialDisposable.isDisposed;

let raiseIfDisposed = continuation =>
  continuation |> asSerialDisposable |> SerialDisposable.raiseIfDisposed;

let continueAfter = {
  let getExecutor =
    fun
    | C0(executor, _, _) as self
    | C1(executor, _, _, _) as self
    | C2(executor, _, _, _, _) as self
    | C3(executor, _, _, _, _, _) as self
    | C4(executor, _, _, _, _, _, _) as self
    | C5(executor, _, _, _, _, _, _, _) as self
    | C6(executor, _, _, _, _, _, _, _, _) as self
    | C7(executor, _, _, _, _, _, _, _, _, _) as self => {
        raiseIfDisposed(self);
        executor;
      }
    | Disposed => DisposedException.raise();

  let doWork = (state, continuation) =>
    switch (continuation) {
    | C0(_, _, work) => work(continuation, state)
    | C1(_, _, work, ctx0) => work(ctx0, continuation, state)
    | C2(_, _, work, ctx0, ctx1) => work(ctx0, ctx1, continuation, state)
    | C3(_, _, work, ctx0, ctx1, ctx2) =>
      work(ctx0, ctx1, ctx2, continuation, state)
    | C4(_, _, work, ctx0, ctx1, ctx2, ctx3) =>
      work(ctx0, ctx1, ctx2, ctx3, continuation, state)
    | C5(_, _, work, ctx0, ctx1, ctx2, ctx3, ctx4) =>
      work(ctx0, ctx1, ctx2, ctx3, ctx4, continuation, state)
    | C6(_, _, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) =>
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, continuation, state)
    | C7(_, _, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6) =>
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, continuation, state)
    | Disposed => ()
    };

  let execute = (state, continuation) =>
    if (! isDisposed(continuation)) {
      doWork(state, continuation);
    };

  (~delay, state, continuation) => {
    raiseIfDisposed(continuation);

    let innerDisposableActive =
      continuation |> get |> Disposable.isDisposed |> (!);

    if (innerDisposableActive) {
      /** FIXME: define a real exception type here */
      failwith("continue called multiple times");
    };

    let executor = continuation |> getExecutor;
    executor(~delay, continuation, state, execute);
  };
};

let continue = (state, continuation) =>
  continueAfter(~delay=0.0, state, continuation);
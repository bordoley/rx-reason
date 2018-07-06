type executor('state, 'a) =
  (~delay: float, 'a, 'state, ('state, 'a) => unit) => Disposable.t;

type t('state) =
  | C0(
      executor('state, t('state)),
      SerialDisposable.t,
      ScheduledWork.t('state),
    )
  | C1(
        executor('state, t('state)),
        SerialDisposable.t,
        ScheduledWork.t1('ctx0, 'state),
        'ctx0,
      ): t('state)
  | C2(
        executor('state, t('state)),
        SerialDisposable.t,
        ScheduledWork.t2('ctx0, 'ctx1, 'state),
        'ctx0,
        'ctx1,
      ): t('state)
  | C3(
        executor('state, t('state)),
        SerialDisposable.t,
        ScheduledWork.t3('ctx0, 'ctx1, 'ctx2, 'state),
        'ctx0,
        'ctx1,
        'ctx2,
      ): t('state)
  | C4(
        executor('state, t('state)),
        SerialDisposable.t,
        ScheduledWork.t4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
      ): t('state)
  | C5(
        executor('state, t('state)),
        SerialDisposable.t,
        ScheduledWork.t5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
      ): t('state)
  | C6(
        executor('state, t('state)),
        SerialDisposable.t,
        ScheduledWork.t6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
        'ctx5,
      ): t('state)
  | C7(
        executor('state, t('state)),
        SerialDisposable.t,
        ScheduledWork.t7(
          'ctx0,
          'ctx1,
          'ctx2,
          'ctx3,
          'ctx4,
          'ctx5,
          'ctx6,
          'state,
        ),
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

module Executor = {
  type t('state, 'a) = executor('state, 'a);
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

let getInnerDisposable = continuation =>
  continuation |> asSerialDisposable |> SerialDisposable.getInnerDisposable;

let setInnerDisposable = (disposable, continuation) =>
  continuation
  |> asSerialDisposable
  |> SerialDisposable.setInnerDisposable(disposable);

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

let rec continue = (state, continuation) =>
  continueAfter(~delay=0.0, state, continuation)
and continueAfter = {
  let getExecutor =
    fun
    | C0(executor, _, _)
    | C1(executor, _, _, _)
    | C2(executor, _, _, _, _)
    | C3(executor, _, _, _, _, _)
    | C4(executor, _, _, _, _, _, _)
    | C5(executor, _, _, _, _, _, _, _)
    | C6(executor, _, _, _, _, _, _, _, _)
    | C7(executor, _, _, _, _, _, _, _, _, _) => executor
    | Disposed => DisposedException.raise();

  let doWork = (state, continuation) =>
    switch (continuation) {
    | C0(_, _, work) => work(state)
    | C1(_, _, work, ctx0) => work(ctx0, state)
    | C2(_, _, work, ctx0, ctx1) => work(ctx0, ctx1, state)
    | C3(_, _, work, ctx0, ctx1, ctx2) => work(ctx0, ctx1, ctx2, state)
    | C4(_, _, work, ctx0, ctx1, ctx2, ctx3) =>
      work(ctx0, ctx1, ctx2, ctx3, state)
    | C5(_, _, work, ctx0, ctx1, ctx2, ctx3, ctx4) =>
      work(ctx0, ctx1, ctx2, ctx3, ctx4, state)
    | C6(_, _, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) =>
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state)
    | C7(_, _, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6) =>
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state)
    | Disposed => Done
    };

  let execute = (state, continuation) =>
    if (! isDisposed(continuation)) {
      let result = doWork(state, continuation);
      switch (result) {
      | Done => continuation |> dispose
      | Continue(state) => continuation |> continue(state)
      | ContinueAfter(delay, state) =>
        continuation |> continueAfter(~delay, state)
      };
    };

  (~delay, state, continuation) => {
    let innerDisposableActive =
      continuation |> getInnerDisposable |> Disposable.isDisposed |> (!);

    if (innerDisposableActive) {
      /** FIXME: define a real exception type here */
      failwith("continue called multiple times");
    };

    let executor = continuation |> getExecutor;
    let innerDisposable = executor(~delay, continuation, state, execute);

    continuation |> setInnerDisposable(innerDisposable);
  };
};
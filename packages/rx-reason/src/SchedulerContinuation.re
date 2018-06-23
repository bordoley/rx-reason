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
  (t('state), 'state, ('state, t('state)) => unit) => unit
and t('state) =
  | C0(executor('state), CompositeDisposable.t, work('state))
  | C1(executor('state), CompositeDisposable.t, work1('state, 'ctx0), 'ctx0): t(
                                                                    'state,
                                                                    )
  | C2(
        executor('state),
        CompositeDisposable.t,
        work2('state, 'ctx0, 'ctx1),
        'ctx0,
        'ctx1,
      ): t('state)
  | C3(
        executor('state),
        CompositeDisposable.t,
        work3('state, 'ctx0, 'ctx1, 'ctx2),
        'ctx0,
        'ctx1,
        'ctx2,
      ): t('state)
  | C4(
        executor('state),
        CompositeDisposable.t,
        work4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
      ): t('state)
  | C5(
        executor('state),
        CompositeDisposable.t,
        work5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
      ): t('state)
  | C6(
        executor('state),
        CompositeDisposable.t,
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
        CompositeDisposable.t,
        work7('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6),
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
        'ctx5,
        'ctx6,
      ): t('state);

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

let asCompositeDisposable =
  fun
  | C0(_, disposable, _)
  | C1(_, disposable, _, _)
  | C2(_, disposable, _, _, _)
  | C3(_, disposable, _, _, _, _)
  | C4(_, disposable, _, _, _, _, _)
  | C5(_, disposable, _, _, _, _, _, _)
  | C6(_, disposable, _, _, _, _, _, _, _)
  | C7(_, disposable, _, _, _, _, _, _, _, _) => disposable;

let addDisposable = (disposable, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addDisposable(disposable)
  |> ignore;
  continuation;
};

let addCompositeDisposable = (disposable, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addCompositeDisposable(disposable)
  |> ignore;
  continuation;
};

let addSerialDisposable = (disposable, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addSerialDisposable(disposable)
  |> ignore;
  continuation;
};

let addTeardown = (teardown, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown(teardown)
  |> ignore;
  continuation;
};

let addTeardown1 = (teardown, d0, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown1(teardown, d0)
  |> ignore;
  continuation;
};

let addTeardown2 = (teardown, d0, d1, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown2(teardown, d0, d1)
  |> ignore;
  continuation;
};

let addTeardown3 = (teardown, d0, d1, d2, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown3(teardown, d0, d1, d2)
  |> ignore;
  continuation;
};

let addTeardown4 = (teardown, d0, d1, d2, d3, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown4(teardown, d0, d1, d2, d3)
  |> ignore;
  continuation;
};

let addTeardown5 = (teardown, d0, d1, d2, d3, d4, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown5(teardown, d0, d1, d2, d3, d4)
  |> ignore;
  continuation;
};

let addTeardown6 = (teardown, d0, d1, d2, d3, d4, d5, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown6(teardown, d0, d1, d2, d3, d4, d5)
  |> ignore;
  continuation;
};

let addTeardown7 = (teardown, d0, d1, d2, d3, d4, d5, d6, continuation) => {
  continuation
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown7(teardown, d0, d1, d2, d3, d4, d5, d6)
  |> ignore;
  continuation;
};

let asDisposable = continuation =>
  continuation |> asCompositeDisposable |> CompositeDisposable.asDisposable;

let create = (executor, work) =>
  C0(executor, CompositeDisposable.create(), work);

let create1 = (executor, work, ctx0) =>
  C1(executor, CompositeDisposable.create(), work, ctx0);

let create2 = (executor, work, ctx0, ctx1) =>
  C2(executor, CompositeDisposable.create(), work, ctx0, ctx1);

let create3 = (executor, work, ctx0, ctx1, ctx2) =>
  C3(executor, CompositeDisposable.create(), work, ctx0, ctx1, ctx2);

let create4 = (executor, work, ctx0, ctx1, ctx2, ctx3) =>
  C4(executor, CompositeDisposable.create(), work, ctx0, ctx1, ctx2, ctx3);

let create5 = (executor, work, ctx0, ctx1, ctx2, ctx3, ctx4) =>
  C5(
    executor,
    CompositeDisposable.create(),
    work,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
  );

let create6 = (executor, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) =>
  C6(
    executor,
    CompositeDisposable.create(),
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
    CompositeDisposable.create(),
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
  continuation |> asCompositeDisposable |> CompositeDisposable.dispose;

let isDisposed = continuation =>
  continuation |> asCompositeDisposable |> CompositeDisposable.isDisposed;

let raiseIfDisposed = continuation =>
  continuation |> asCompositeDisposable |> CompositeDisposable.raiseIfDisposed;

let continue = {
  let getExecutor =
    fun
    | C0(executor, _, _)
    | C1(executor, _, _, _)
    | C2(executor, _, _, _, _)
    | C3(executor, _, _, _, _, _)
    | C4(executor, _, _, _, _, _, _)
    | C5(executor, _, _, _, _, _, _, _)
    | C6(executor, _, _, _, _, _, _, _, _)
    | C7(executor, _, _, _, _, _, _, _, _, _) => executor;

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
    };

  (state, continuation) =>
    if (! isDisposed(continuation)) {
      let executor = continuation |> getExecutor;
      executor(continuation, state, doWork);
    };
};
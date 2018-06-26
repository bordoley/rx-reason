/** FIXME: In ocaml >4.06 you can embed records in GADTs and use mutable fields instead of refs  */
type t =
  | Disposable(TeardownLogic.t, ref(bool))
  | Disposable1(TeardownLogic.t1('ctx0), 'ctx0, ref(bool)): t
  | Disposable2(TeardownLogic.t2('ctx0, 'ctx1), 'ctx0, 'ctx1, ref(bool)): t
  | Disposable3(
                 TeardownLogic.t3('ctx0, 'ctx1, 'ctx2),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 ref(bool),
               ): t
  | Disposable4(
                 TeardownLogic.t4('ctx0, 'ctx1, 'ctx2, 'ctx3),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 ref(bool),
               ): t
  | Disposable5(
                 TeardownLogic.t5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 ref(bool),
               ): t
  | Disposable6(
                 TeardownLogic.t6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 'ctx5,
                 ref(bool),
               ): t
  | Disposable7(
                 TeardownLogic.t7(
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   'ctx6,
                 ),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 'ctx5,
                 'ctx6,
                 ref(bool),
               ): t;
type disposable = t;

module type S = {
  include DisposableLike.S;

  let asDisposable: t => disposable;
};

module type S1 = {
  include DisposableLike.S1;

  let asDisposable: t('a) => disposable;
};

let create = teardown : t => Disposable(teardown, ref(false));

let create1 = (teardown, d0) : t => Disposable1(teardown, d0, ref(false));

let create2 = (teardown, d0, d1) : t =>
  Disposable2(teardown, d0, d1, ref(false));

let create3 = (teardown, d0, d1, d2) : t =>
  Disposable3(teardown, d0, d1, d2, ref(false));

let create4 = (teardown, d0, d1, d2, d3) : t =>
  Disposable4(teardown, d0, d1, d2, d3, ref(false));

let create5 = (teardown, d0, d1, d2, d3, d4) : t =>
  Disposable5(teardown, d0, d1, d2, d3, d4, ref(false));

let create6 = (teardown, d0, d1, d2, d3, d4, d5) : t =>
  Disposable6(teardown, d0, d1, d2, d3, d4, d5, ref(false));

let create7 = (teardown, d0, d1, d2, d3, d4, d5, d6) : t =>
  Disposable7(teardown, d0, d1, d2, d3, d4, d5, d6, ref(false));

let dispose = {
  let shouldDispose =
    fun
    | Disposable(_, isDisposed)
    | Disposable1(_, _, isDisposed)
    | Disposable2(_, _, _, isDisposed)
    | Disposable3(_, _, _, _, isDisposed)
    | Disposable4(_, _, _, _, _, isDisposed)
    | Disposable5(_, _, _, _, _, _, isDisposed)
    | Disposable6(_, _, _, _, _, _, _, isDisposed)
    | Disposable7(_, _, _, _, _, _, _, _, isDisposed) =>
      ! Interlocked.exchange(true, isDisposed);

  let doTeardown =
    fun
    | Disposable(teardown, _) => teardown()
    | Disposable1(teardown, a0, _) => teardown(a0)
    | Disposable2(teardown, a0, a1, _) => teardown(a0, a1)
    | Disposable3(teardown, a0, a1, a2, _) => teardown(a0, a1, a2)
    | Disposable4(teardown, a0, a1, a2, a3, _) => teardown(a0, a1, a2, a3)
    | Disposable5(teardown, a0, a1, a2, a3, a4, _) =>
      teardown(a0, a1, a2, a3, a4)
    | Disposable6(teardown, a0, a1, a2, a3, a4, a5, _) =>
      teardown(a0, a1, a2, a3, a4, a5)
    | Disposable7(teardown, a0, a1, a2, a3, a4, a5, a6, _) =>
      teardown(a0, a1, a2, a3, a4, a5, a6);

  disposable => {
    let shouldDispose = shouldDispose(disposable);
    if (shouldDispose) {
      doTeardown(disposable);
    };
  };
};

let compose = {
  let dispose = List.iter(dispose);
  disposables => create1(dispose, disposables);
};

let empty = () => create(Functions.alwaysUnit1);

let disposed: t = {
  let retval = empty();
  retval |> dispose |> ignore;
  retval;
};

let isDisposed =
  fun
  | Disposable(_, isDisposed)
  | Disposable1(_, _, isDisposed)
  | Disposable2(_, _, _, isDisposed)
  | Disposable3(_, _, _, _, isDisposed)
  | Disposable4(_, _, _, _, _, isDisposed)
  | Disposable5(_, _, _, _, _, _, isDisposed)
  | Disposable6(_, _, _, _, _, _, _, isDisposed)
  | Disposable7(_, _, _, _, _, _, _, _, isDisposed) =>
    Volatile.read(isDisposed);

let raiseIfDisposed = (disposable: t) =>
  if (isDisposed(disposable)) {
    DisposedException.raise();
  };
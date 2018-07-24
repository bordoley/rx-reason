type t =
  | Disposable(Atomic.t(bool), TeardownLogic.t)
  | Disposable1(Atomic.t(bool), TeardownLogic.t1('ctx0), 'ctx0): t
  | Disposable2(
                 Atomic.t(bool),
                 TeardownLogic.t2('ctx0, 'ctx1),
                 'ctx0,
                 'ctx1,
               ): t
  | Disposable3(
                 Atomic.t(bool),
                 TeardownLogic.t3('ctx0, 'ctx1, 'ctx2),
                 'ctx0,
                 'ctx1,
                 'ctx2,
               ): t
  | Disposable4(
                 Atomic.t(bool),
                 TeardownLogic.t4('ctx0, 'ctx1, 'ctx2, 'ctx3),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
               ): t
  | Disposable5(
                 Atomic.t(bool),
                 TeardownLogic.t5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
               ): t
  | Disposable6(
                 Atomic.t(bool),
                 TeardownLogic.t6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 'ctx5,
               ): t
  | Disposable7(
                 Atomic.t(bool),
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
               ): t
  | Disposed
  | Empty(Atomic.t(bool));

type disposable = t;

module type S = {
  include DisposableLike.S;

  let asDisposable: t => disposable;
};

module type S1 = {
  include DisposableLike.S1;

  let asDisposable: t('a) => disposable;
};

let create = teardown : t => Disposable(Atomic.make(false), teardown);

let create1 = (teardown, d0) : t =>
  Disposable1(Atomic.make(false), teardown, d0);

let create2 = (teardown, d0, d1) : t =>
  Disposable2(Atomic.make(false), teardown, d0, d1);

let create3 = (teardown, d0, d1, d2) : t =>
  Disposable3(Atomic.make(false), teardown, d0, d1, d2);

let create4 = (teardown, d0, d1, d2, d3) : t =>
  Disposable4(Atomic.make(false), teardown, d0, d1, d2, d3);

let create5 = (teardown, d0, d1, d2, d3, d4) : t =>
  Disposable5(Atomic.make(false), teardown, d0, d1, d2, d3, d4);

let create6 = (teardown, d0, d1, d2, d3, d4, d5) : t =>
  Disposable6(Atomic.make(false), teardown, d0, d1, d2, d3, d4, d5);

let create7 = (teardown, d0, d1, d2, d3, d4, d5, d6) : t =>
  Disposable7(Atomic.make(false), teardown, d0, d1, d2, d3, d4, d5, d6);

let dispose = {
  let shouldDispose =
    fun
    | Disposable(isDisposed, _)
    | Disposable1(isDisposed, _, _)
    | Disposable2(isDisposed, _, _, _)
    | Disposable3(isDisposed, _, _, _, _)
    | Disposable4(isDisposed, _, _, _, _, _)
    | Disposable5(isDisposed, _, _, _, _, _, _)
    | Disposable6(isDisposed, _, _, _, _, _, _, _)
    | Disposable7(isDisposed, _, _, _, _, _, _, _, _)
    | Empty(isDisposed) => ! Atomic.exchange(isDisposed, true)
    | Disposed => false;

  let doTeardown =
    fun
    | Disposable(_, teardown) => teardown()
    | Disposable1(_, teardown, a0) => teardown(a0)
    | Disposable2(_, teardown, a0, a1) => teardown(a0, a1)
    | Disposable3(_, teardown, a0, a1, a2) => teardown(a0, a1, a2)
    | Disposable4(_, teardown, a0, a1, a2, a3) => teardown(a0, a1, a2, a3)
    | Disposable5(_, teardown, a0, a1, a2, a3, a4) =>
      teardown(a0, a1, a2, a3, a4)
    | Disposable6(_, teardown, a0, a1, a2, a3, a4, a5) =>
      teardown(a0, a1, a2, a3, a4, a5)
    | Disposable7(_, teardown, a0, a1, a2, a3, a4, a5, a6) =>
      teardown(a0, a1, a2, a3, a4, a5, a6)
    | Empty(_)
    | Disposed => ();

  disposable => {
    let shouldDispose = shouldDispose(disposable);
    if (shouldDispose) {
      doTeardown(disposable);
    };
  };
};

let compose = {
  let dispose = Lists.iter(dispose);
  disposables => create1(dispose, disposables);
};

let empty = () => Empty(Atomic.make(false));

let disposed: t = Disposed;

let isDisposed =
  fun
  | Disposable(isDisposed, _)
  | Disposable1(isDisposed, _, _)
  | Disposable2(isDisposed, _, _, _)
  | Disposable3(isDisposed, _, _, _, _)
  | Disposable4(isDisposed, _, _, _, _, _)
  | Disposable5(isDisposed, _, _, _, _, _, _)
  | Disposable6(isDisposed, _, _, _, _, _, _, _)
  | Disposable7(isDisposed, _, _, _, _, _, _, _, _)
  | Empty(isDisposed) => Atomic.get(isDisposed)
  | Disposed => true;

let raiseIfDisposed = (disposable: t) =>
  if (isDisposed(disposable)) {
    DisposedException.raise();
  };
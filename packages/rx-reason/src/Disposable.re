/** FIXME: In ocaml >4.06 you can embed records in GADTs and use mutable fields instead of refs  */
type t =
  | Disposable(unit => unit, ref(bool))
  | Disposable1('ctx0 => unit, 'ctx0, ref(bool)): t
  | Disposable2(('ctx0, 'ctx1) => unit, 'ctx0, 'ctx1, ref(bool)): t;

type disposable = t;

module type S = {
  type t;

  let asDisposable: t => disposable;
  let dispose: t => unit;
  let isDisposed: t => bool;
  let raiseIfDisposed: t => unit;
};

module type S1 = {
  type t('a);

  let asDisposable: t('a) => disposable;
  let dispose: t('a) => unit;
  let isDisposed: t('a) => bool;
  let raiseIfDisposed: t('a) => unit;
};

let asDisposable = Functions.identity;

let create = onDispose : t => Disposable(onDispose, ref(false));

let create1 = (onDispose, d0) : t => Disposable1(onDispose, d0, ref(false));

let create2 = (onDispose, d0, d1) : t =>
  Disposable2(onDispose, d0, d1, ref(false));

let dispose = {
  let shouldDispose =
    fun
    | Disposable(_, isDisposed)
    | Disposable1(_, _, isDisposed)
    | Disposable2(_, _, _, isDisposed) =>
      ! Interlocked.exchange(true, isDisposed);

  let doDispose =
    fun
    | Disposable(dispose, _) => dispose()
    | Disposable1(dispose, a0, _) => dispose(a0)
    | Disposable2(dispose, a0, a1, _) => dispose(a0, a1);

  disposable => {
    let shouldDispose = shouldDispose(disposable);
    if (shouldDispose) {
      doDispose(disposable);
    };
  };
};

let compose = {
  let dispose = List.iter(Functions.(dispose >> ignore));
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
  | Disposable2(_, _, _, isDisposed) => Volatile.read(isDisposed);

let raiseIfDisposed = (disposable: t) =>
  if (isDisposed(disposable)) {
    DisposedException.raise();
  };
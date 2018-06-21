type t = {
  onDispose: ref(unit => unit),
  isDisposed: ref(bool),
};

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

let create = (onDispose) : t => {
  onDispose: ref(onDispose),
  isDisposed: ref(false),
};

let dispose = ({onDispose, isDisposed}: t) : unit => {
  let shouldDispose = ! Interlocked.exchange(true, isDisposed);
  if (shouldDispose) {
    let onDispose = Interlocked.exchange(Functions.alwaysUnit1, onDispose);
    onDispose();
  };
};

let compose = (disposables: list(t)) : t => {
  let dispose = () => disposables |> List.iter(Functions.(dispose >> ignore));
  create(dispose);
};

let empty = () => create(Functions.alwaysUnit1);

let disposed: t = {
  let retval = empty();
  retval |> dispose |> ignore;
  retval;
};

let isDisposed = ({isDisposed}: t) : bool => Volatile.read(isDisposed);

let raiseIfDisposed = (disposable: t)  =>
  if (isDisposed(disposable)) {
    DisposedException.raise();
  };
type t;
type disposable = t;

module type S = {
  type t;

  let dispose: t => unit;
  let isDisposed: t => bool;
  let toDisposable: t => disposable;
};

module type S1 = {
  type t('a);

  let dispose: t('a) => unit;
  let isDisposed: t('a) => bool;
  let toDisposable: t('a) => disposable;
};

include S with type t := t;

exception DisposedException;

let compose: list(t) => t;

let create: (unit => unit) => t;

let disposed: t;

let empty: unit => t;

let raiseIfDisposed: t => unit;
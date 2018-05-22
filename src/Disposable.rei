type t;

exception DisposedException;

let compose: list(t) => t;

let create: (unit => unit) => t;

let dispose: t => unit;

let disposeWithResult: t => bool;

let disposed: t;

let empty: unit => t;

let isDisposed: t => bool;

let raiseIfDisposed: t => unit;
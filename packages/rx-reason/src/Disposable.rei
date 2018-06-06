type t;

let compose: list(t) => t;

let create: (unit => unit) => t;

let dispose: t => unit;

let disposed: t;

let empty: unit => t;

let isDisposed: t => bool;
type t;

let create: unit =>t;
let dispose: t => unit;
let isDisposed: t => bool;
let set: (Disposable.t, t) => unit;
let toDisposable: t => Disposable.t;
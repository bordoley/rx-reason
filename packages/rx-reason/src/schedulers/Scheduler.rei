module rec Continuation: {
  type t = (~now: unit => float, ~shouldYield: unit => bool) => Result.t;
}
and Result: {
  type t;

  let yield: Continuation.t => t;
  let continueAfter: (~delay: float, Continuation.t) => t;
  let complete: t;

  let continueWith: ((~delay: float, Continuation.t) => unit, t) => unit;

  let flatMapToOption:
    ((~delay: float, Continuation.t) => option('a), t) => option('a);
};

type t = {
  now: unit => float,
  schedule: (~delay: float, Continuation.t) => Disposable.t,
};

let now: t => float;

let schedule: (~delay: float=?, Continuation.t, t) => Disposable.t;
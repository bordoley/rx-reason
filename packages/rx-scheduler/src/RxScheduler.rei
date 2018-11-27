module rec Continuation: {
  type t = (~now: unit => float, ~shouldYield: unit => bool) => Result.t;
}
and Result: {
  type t =
    | Yield(Continuation.t)
    | ContinueAfter(float, Continuation.t)
    | Complete;

  let yield: Continuation.t => t;
  let continueAfter: (~delay: float, Continuation.t) => t;
  let complete: t;

  let continueWith: ((~delay: float=?, Continuation.t) => unit, t) => unit;
};

type t = {
  now: unit => float,
  schedule: (~delay: float=?, Continuation.t) => RxDisposable.t,
};

let now: t => float;

let schedule: (~delay: float=?, Continuation.t, t) => RxDisposable.t;
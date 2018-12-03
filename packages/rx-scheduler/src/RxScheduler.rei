module rec Continuation: {
  type t = (~now: unit => float, ~shouldYield: unit => bool) => Result.t;
}
and Result: {
  type t;

  let yield: Continuation.t => t;
  let continueAfter: (~delay: float, Continuation.t) => t;
  let complete: t;

  let map:
    (
      ~onYield: Continuation.t => 'a,
      ~onContinueAfter: (~delay: float, Continuation.t) => 'a,
      ~onComplete: unit => 'a,
      t
    ) =>
    'a;

  let map1:
    (
      ~onYield: ('ctx0, Continuation.t) => 'a,
      ~onContinueAfter: ('ctx0, ~delay: float, Continuation.t) => 'a,
      ~onComplete: 'ctx0 => 'a,
      'ctx0,
      t
    ) =>
    'a;

  let map2:
    (
      ~onYield: ('ctx0, 'ctx1, Continuation.t) => 'a,
      ~onContinueAfter: ('ctx0, 'ctx1, ~delay: float, Continuation.t) => 'a,
      ~onComplete: ('ctx0, 'ctx1) => 'a,
      'ctx0,
      'ctx1,
      t
    ) =>
    'a;

  let map3:
    (
      ~onYield: ('ctx0, 'ctx1, 'ctx2, Continuation.t) => 'a,
      ~onContinueAfter: ('ctx0, 'ctx1, 'ctx2, ~delay: float, Continuation.t) =>
                        'a,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2) => 'a,
      'ctx0,
      'ctx1,
      'ctx2,
      t
    ) =>
    'a;

    let map4:
    (
      ~onYield: ('ctx0, 'ctx1, 'ctx2, 'ctx3, Continuation.t) => 'a,
      ~onContinueAfter: ('ctx0, 'ctx1, 'ctx2, 'ctx3, ~delay: float, Continuation.t) =>
                        'a,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3) => 'a,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      t
    ) =>
    'a;
};

type t = {
  now: unit => float,
  schedule: (~delay: float=?, Continuation.t) => RxDisposable.t,
};

let now: t => float;

let schedule: (~delay: float=?, Continuation.t, t) => RxDisposable.t;
type continuation = (~now: unit => float, ~shouldYield: unit => bool) => result
and result =
  | Yield(continuation)
  | ContinueAfter(float, continuation)
  | Complete;

module Continuation = {
  type t = continuation;
};

module Result = {
  type t =
    result =
      | Yield(continuation) | ContinueAfter(float, continuation) | Complete;

  let yield = continuation => Yield(continuation);

  let continueAfter = (~delay, continuation) => {
    RxPreconditions.checkArgument(
      delay > 0.0,
      "RxScheduler: delay must be greater than 0.0",
    );
    ContinueAfter(delay, continuation);
  };

  let complete = Complete;

  let map = (~onYield, ~onContinueAfter, ~onComplete, result) =>
    switch (result) {
    | Yield(continuation) => onYield(continuation)
    | ContinueAfter(delay, continuation) =>
      onContinueAfter(~delay, continuation)
    | Complete => onComplete()
    };

  let map1 = (~onYield, ~onContinueAfter, ~onComplete, ctx0, result) =>
    switch (result) {
    | Yield(continuation) => onYield(ctx0, continuation)
    | ContinueAfter(delay, continuation) =>
      onContinueAfter(ctx0, ~delay, continuation)
    | Complete => onComplete(ctx0)
    };

  let map2 = (~onYield, ~onContinueAfter, ~onComplete, ctx0, ctx1, result) =>
    switch (result) {
    | Yield(continuation) => onYield(ctx0, ctx1, continuation)
    | ContinueAfter(delay, continuation) =>
      onContinueAfter(ctx0, ctx1, ~delay, continuation)
    | Complete => onComplete(ctx0, ctx1)
    };

  let map3 =
      (~onYield, ~onContinueAfter, ~onComplete, ctx0, ctx1, ctx2, result) =>
    switch (result) {
    | Yield(continuation) => onYield(ctx0, ctx1, ctx2, continuation)
    | ContinueAfter(delay, continuation) =>
      onContinueAfter(ctx0, ctx1, ctx2, ~delay, continuation)
    | Complete => onComplete(ctx0, ctx1, ctx2)
    };

  let map4 =
      (
        ~onYield,
        ~onContinueAfter,
        ~onComplete,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        result,
      ) =>
    switch (result) {
    | Yield(continuation) => onYield(ctx0, ctx1, ctx2, ctx3, continuation)
    | ContinueAfter(delay, continuation) =>
      onContinueAfter(ctx0, ctx1, ctx2, ctx3, ~delay, continuation)
    | Complete => onComplete(ctx0, ctx1, ctx2, ctx3)
    };
};

type t = {
  now: unit => float,
  schedule: (~delay: float=?, Continuation.t) => RxDisposable.t,
};

let now = scheduler => scheduler.now();

let schedule = (~delay=?, continuation, scheduler) =>
  scheduler.schedule(~delay?, continuation);
type continuation = (~now: unit => float, ~shouldYield: unit => bool) => result
and result =
  | Yield(continuation)
  | ContinueAfter(float, continuation)
  | Complete;

module Continuation = {
  type t = continuation;
};

module Result = {
  type t = result = 
    | Yield(continuation)
    | ContinueAfter(float, continuation)
    | Complete;
  
  let yield = continuation => Yield(continuation);
  let continueAfter = (~delay, continuation) => ContinueAfter(delay, continuation);
  let complete = Complete;

  type continueWithCb = (~delay: float=?, continuation) => unit;

  let continueWith = (cb: continueWithCb, result) =>
    switch (result) {
    | Yield(continuation) => cb(continuation)
    | ContinueAfter(delay, continuation) => cb(~delay, continuation)
    | _ => ()
    };
};

type t = {
  now: unit => float,
  schedule: (~delay: float=?, Continuation.t) => Disposable.t,
};

let now = scheduler => scheduler.now();

let schedule = (~delay=?, continuation, scheduler) =>
  scheduler.schedule(~delay?, continuation);
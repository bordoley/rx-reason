type continuation = (~shouldYield: unit => bool) => result
and result =
  | Yield(float, continuation)
  | Complete;

module Continuation = {
  type t = continuation;
};

module Result = {
  type t = result;

  let yield = continuation => Yield(0.0, continuation);
  let continueAfter = (~delay, continuation) => Yield(delay, continuation);
  let complete = Complete;

  let continueWith = (cb, result) =>
    switch (result) {
    | Yield(delay, continuation) => cb(~delay, continuation)
    | _ => ()
    };

  let flatMapToOption = (mapper, result) =>
    switch (result) {
    | Yield(delay, continuation) => mapper(~delay, continuation)
    | _ => None
    };
};

type t = {
  now: unit => float,
  schedule: (~delay: float, Continuation.t) => Disposable.t,
};

let now = scheduler => scheduler.now();

let schedule = (~delay=0.0, continuation, scheduler) =>
  scheduler.schedule(~delay, continuation);
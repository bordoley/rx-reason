[@bs.val]
external setInterval1 : ('a => unit, float, 'a) => Js.Global.intervalId =
  "setInterval";

[@bs.val]
external setTimeout1 : ('a => unit, float, 'a) => Js.Global.timeoutId =
  "setTimeout";

[@bs.send.pipe: Js.Promise.t('a)]
external then_ : ([@bs.uncurry] ('a => unit)) => Js.Promise.t(unit) = "then";

let clearInterval = intervalId => Js.Global.clearInterval(intervalId);
let clearTimeout = timeoutId => Js.Global.clearTimeout(timeoutId);

let promiseContinuation = ((disposable, f, state)) =>
  if (! RxReason.Disposable.isDisposed(disposable)) {
    f(state);
  };

let scheduleAfter = (~delay, f, state) =>
  if (delay > 0.0) {
    let timeoutId = setTimeout1(f, delay, state);
    RxReason.Disposable.create1(clearTimeout, timeoutId);
  } else {
    let disposable = RxReason.Disposable.empty();
    Js.Promise.resolve((disposable, f, state))
    |> then_(promiseContinuation)
    |> ignore;
    disposable;
  };

let schedulePeriodic = (~delay, f, state) => {
  let intervalId = setInterval1(f, delay, state);
  RxReason.Disposable.create1(clearInterval, intervalId);
};

let scheduler: RxReason.Scheduler.t = {
  now: Js.Date.now,
  scheduleAfter,
  schedulePeriodic,
};
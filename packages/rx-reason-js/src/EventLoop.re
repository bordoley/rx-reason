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

let scheduler: RxReason.Scheduler.t = {
  now: Js.Date.now,
  scheduleAfter: (~delay, f, state) =>
    if (delay > 0.0) {
      let timeoutId = setTimeout1(f, delay, state);
      RxReason.Disposable.create1(clearTimeout, timeoutId);
    } else {
      Js.Promise.resolve(state) |> then_(f) |> ignore;
      RxReason.Disposable.disposed;
    },
  schedulePeriodic: (~delay, f, state) => {
    let intervalId = setInterval1(f, delay, state);
    RxReason.Disposable.create1(clearInterval, intervalId);
  },
};
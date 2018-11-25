let immediatePriority = 1;
let userBlockingPriority = 2;
let normalPriority = 3;
let lowPriority = 4;
let idlePriority = 5;

type callbackHandle;

type fn;

type callback = unit => fn;

[@bs.val] [@bs.module "scheduler"]
external runWithPriority : (int, unit => 't) => 't =
  "unstable_runWithPriority";

[@bs.val] [@bs.module "scheduler"]
external schedulerScheduleCallback : callback => callbackHandle =
  "unstable_scheduleCallback";

[@bs.val] [@bs.module "scheduler"]
external schedulerCancelCallback : callbackHandle => unit =
  "unstable_cancelCallback";

[@bs.val] [@bs.module "scheduler"]
external schedulerShouldYield : unit => bool = "unstable_shouldYield";

[@bs.val] [@bs.module "scheduler"]
external schedulerNow : unit => float = "unstable_now";

[@bs.val]
external setTimeout : ('a, float, 'b, 'c, 'd) => Js.Global.timeoutId =
  "setTimeout";

let cancelCallback = handle => schedulerCancelCallback(handle);
let clearTimeout = timeoutId => Js.Global.clearTimeout(timeoutId);
let now = () => schedulerNow();
let scheduleCallback = (cb, ()) => schedulerScheduleCallback(cb);
let shouldYield = () => schedulerShouldYield();

let rec scheduleNowWithPriority = (disposable, priority, continuation) => {
  let isDisposed = RxReason.SerialDisposable.isDisposed(disposable);

  if (! isDisposed) {
    let rec callback =
            (continuation: RxReason.Scheduler.Continuation.t, ())
            : option(callback) => {
      let result = continuation(~now, ~shouldYield);
      
      switch (result) {
      | Yield(continuation) => Some(Obj.magic(callback(continuation)))
      | ContinueAfter(delay, continuation) =>
        scheduleInternal(disposable, priority, ~delay, continuation);
        None;
      | Complete => None
      };
    };

    let callbackHandle =
      runWithPriority(
        priority,
        scheduleCallback(Obj.magic(callback(continuation))),
      );
    let innerDisposable =
      RxReason.Disposable.create1(cancelCallback, callbackHandle);

    disposable
    |> RxReason.SerialDisposable.setInnerDisposable(innerDisposable);
  };
}
and scheduleInternal = (disposable, priority, ~delay=?, continuation) => {
  disposable
  |> RxReason.SerialDisposable.getInnerDisposable
  |> RxReason.Disposable.dispose;

  switch (delay) {
  | Some(delay) when delay > 0.0 =>
    /**
     * FIXME: Ideally we'd use the same setInterval trick used in the eventloop scheduler.
     * To do so would involve sending tracking the elapsed time per interval
     * and comparing it to the desired delay from the previous executed scheduler action.
     * Using some slosh we could take advantage of an existing setInterval.
     * Might also have to play tricks with keeping track of both the interval's subscription
     * and the scheduler's subscription. Seemed tricky, so use setTimeout instead for now.
     *
     * see:
     * https://github.com/ReactiveX/rxjs/blob/master/src/internal/scheduler/AsyncAction.ts
     */

    let timeoutId =
      setTimeout(
        scheduleNowWithPriority,
        delay,
        priority,
        disposable,
        continuation,
      );
    let innerDisposable =
      RxReason.Disposable.create1(clearTimeout, timeoutId);
    disposable
    |> RxReason.SerialDisposable.setInnerDisposable(innerDisposable);

  | _ => scheduleNowWithPriority(disposable, priority, continuation)
  };
};

let schedule = (priority, ~delay=?, continuation) => {
  let disposable = RxReason.SerialDisposable.create();
  scheduleInternal(disposable, priority, ~delay?, continuation);
  disposable |> RxReason.SerialDisposable.asDisposable;
};

let immediate: RxReason.Scheduler.t = {
  now,
  schedule: (~delay=?, continuation) =>
    schedule(immediatePriority, ~delay?, continuation),
};

let userBlocking: RxReason.Scheduler.t = {
  now,
  schedule: (~delay=?, continuation) =>
    schedule(userBlockingPriority, ~delay?, continuation),
};

let normal: RxReason.Scheduler.t = {
  now,
  schedule: (~delay=?, continuation) =>
    schedule(normalPriority, ~delay?, continuation),
};

let low: RxReason.Scheduler.t = {
  now,
  schedule: (~delay=?, continuation) =>
    schedule(lowPriority, ~delay?, continuation),
};

let idle: RxReason.Scheduler.t = {
  now,
  schedule: (~delay=?, continuation) =>
    schedule(idlePriority, ~delay?, continuation),
};
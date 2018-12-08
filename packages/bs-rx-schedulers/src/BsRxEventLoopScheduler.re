[@bs.val]
external setInterval : ('a, float, 'b, 'c, 'd) => Js.Global.intervalId =
  "setInterval";

[@bs.send.pipe: Js.Promise.t('a)]
external then_ : ([@bs.uncurry] ('a => unit)) => Js.Promise.t(unit) = "then";

let clearInterval = intervalId => Js.Global.clearInterval(intervalId);

let now = () => Js.Date.now();

let rec promiseContinuation = ctx => {
  let (shouldYield, disposable, delayRef, continuationRef) = ctx;

  if (! RxSerialDisposable.isDisposed(disposable)) {
    continuationRef^(~now, ~shouldYield)
    |> RxScheduler.Result.map4(
         ~onYield,
         ~onContinueAfter,
         ~onComplete,
         shouldYield,
         disposable,
         delayRef,
         continuationRef,
       );
  };
}
and scheduleNow = (shouldYield, disposable, delayRef, continuationRef) =>
  Js.Promise.resolve((shouldYield, disposable, delayRef, continuationRef))
  |> then_(promiseContinuation)
  |> ignore
and onYield = (shouldYield, disposable, _, continuationRef, continuation) => {
  continuationRef := continuation;
  scheduleInternal(shouldYield, disposable, ~delay=0.0, continuation);
}
and onContinueAfter =
    (shouldYield, disposable, delayRef, continuationRef, ~delay, continuation) => {
  continuationRef := continuation;
  if (delayRef^ != delay) {
    scheduleInternal(shouldYield, disposable, ~delay, continuation);
  };
}
and onComplete = (_, disposable, _, _) =>
  disposable |> RxSerialDisposable.dispose
and intervalContinuation =
    (shouldYield, disposable, delayRef, continuationRef) =>
  if (! RxSerialDisposable.isDisposed(disposable)) {
    continuationRef^(~now, ~shouldYield)
    |> RxScheduler.Result.map4(
         ~onYield,
         ~onContinueAfter,
         ~onComplete,
         shouldYield,
         disposable,
         delayRef,
         continuationRef,
       );
  }
and scheduleInternal = (shouldYield, disposable, ~delay=?, continuation) => {
  disposable |> RxSerialDisposable.getInnerDisposable |> RxDisposable.dispose;

  switch (delay) {
  | Some(delay) when delay > 0.0 =>
    let intervalId =
      setInterval(
        intervalContinuation,
        delay,
        shouldYield,
        ref(delay),
        ref(continuation),
      );
    let innerDisposable = RxDisposable.create1(clearInterval, intervalId);
    disposable |> RxSerialDisposable.setInnerDisposable(innerDisposable);
  | _ => scheduleNow(shouldYield, disposable, ref(0.0), ref(continuation))
  };
};

let instance: RxScheduler.t = {
  now,
  schedule: (~delay=?, continuation) => {
    let disposable = RxSerialDisposable.create();
    let shouldYield = () => ! RxSerialDisposable.isDisposed(disposable);
    scheduleInternal(shouldYield, disposable, ~delay?, continuation);
    disposable |> RxSerialDisposable.asDisposable;
  },
};
[@bs.val]
external setInterval : ('a, float, 'b, 'c) => Js.Global.intervalId =
  "setInterval";

[@bs.send.pipe: Js.Promise.t('a)]
external then_ : ([@bs.uncurry] ('a => unit)) => Js.Promise.t(unit) = "then";

let clearInterval = intervalId => Js.Global.clearInterval(intervalId);

let shouldYield = () => true;

let now = () => Js.Date.now();

let rec promiseContinuation = ctx => {
  let (disposable, continuation) = ctx;

  if (! RxSerialDisposable.isDisposed(disposable)) {
    continuation(~now, ~shouldYield)
    |> RxScheduler.Result.continueWith(scheduleInternal(disposable));
  };
}
and scheduleNow = (disposable, continuation) =>
  Js.Promise.resolve((disposable, continuation))
  |> then_(promiseContinuation)
  |> ignore
and intervalContinueWithAndSchedulerIfDelayChanged =
    (disposable, delayRef, continuationRef, ~delay=?, continuation) => {
  continuationRef := continuation;
  if (delayRef != delay) {
    scheduleInternal(disposable, ~delay?, continuation);
  };
}
and intervalContinuation = (disposable, delayRef, continuationRef) =>
  if (! RxSerialDisposable.isDisposed(disposable)) {
    continuationRef^(~now, ~shouldYield)
    |> RxScheduler.Result.continueWith(
         intervalContinueWithAndSchedulerIfDelayChanged(
           disposable,
           delayRef,
           continuationRef,
         ),
       );
  }
and scheduleInternal = (disposable, ~delay=?, continuation) => {
  disposable
  |> RxSerialDisposable.getInnerDisposable
  |> RxDisposable.dispose;

  switch (delay) {
  | Some(delay) when delay > 0.0 =>
    let intervalId =
      setInterval(
        intervalContinuation,
        delay,
        ref(delay),
        ref(continuation),
      );
    let innerDisposable =
      RxDisposable.create1(clearInterval, intervalId);
    disposable
    |> RxSerialDisposable.setInnerDisposable(innerDisposable);
  | _ => scheduleNow(disposable, continuation)
  };
};

let instance: RxScheduler.t = {
  now,
  schedule: (~delay=?, continuation) => {
    let disposable = RxSerialDisposable.create();
    scheduleInternal(disposable, ~delay?, continuation);
    disposable |> RxSerialDisposable.asDisposable;
  },
};
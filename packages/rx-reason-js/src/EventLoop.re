let resolveUnit = Js.Promise.resolve();

let schedule: RxReason.Scheduler.t =
  work => {
    let disposable = RxReason.SerialDisposable.create();
    let run = () => {
      disposable
      |> RxReason.SerialDisposable.set(RxReason.Disposable.disposed);
      let isDisposed = disposable |> RxReason.SerialDisposable.isDisposed;
      if (! isDisposed) {
        disposable |> RxReason.SerialDisposable.set(work());
      };
      resolveUnit;
    };
    Js.Promise.(resolveUnit |> then_(run)) |> ignore;
    disposable |> RxReason.SerialDisposable.asDisposable;
  };

let scheduleWithDelay: RxReason.DelayScheduler.t =
  (delay, work) => {
    let disposable = RxReason.SerialDisposable.create();
    let timeoutId =
      Js.Global.setTimeout(
        () => {
          disposable
          |> RxReason.SerialDisposable.set(RxReason.Disposable.disposed);
          let isDisposed = disposable |> RxReason.SerialDisposable.isDisposed;
          if (! isDisposed) {
            disposable |> RxReason.SerialDisposable.set(work());
          };
        },
        int_of_float(delay),
      );
    let timeoutDisposable =
      RxReason.Disposable.create1(Js.Global.clearTimeout, timeoutId);
    disposable |> RxReason.SerialDisposable.set(timeoutDisposable);
    disposable |> RxReason.SerialDisposable.asDisposable;
  };

let clockScheduler: RxReason.ClockScheduler.t = {
  now: Js.Date.now,
  schedule,
  scheduleWithDelay,
};

let schedulerNew: RxReason.SchedulerNew.t = {
  let run = ((continuation, state, f, disposable)) => {
    let shouldRun = ! RxReason.Disposable.isDisposed(disposable);
    disposable |> RxReason.Disposable.dispose;

    if (shouldRun) {
      f(state, continuation);
    };
    resolveUnit;
  };

  let executor = (continuation, state, f) => {
    let disposable = RxReason.Disposable.empty();
    continuation |> RxReason.SchedulerContinuation.set(disposable);

    Js.Promise.resolve((continuation, state, f, disposable))
    |> Js.Promise.then_(run)
    |> ignore;
  };
  {executor: executor};
};
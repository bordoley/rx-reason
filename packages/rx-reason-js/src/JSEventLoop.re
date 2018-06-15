let resolveUnit = Js.Promise.resolve();

let getCurrentTime=Js.Date.now;

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
  (~delay, work) => {
    let disposable = RxReason.SerialDisposable.create();
    let timeoutId =
      Js.Global.setTimeout(
        () => {
          disposable
          |> RxReason.SerialDisposable.set(RxReason.Disposable.disposed);
          let isDisposed =
            disposable |> RxReason.SerialDisposable.isDisposed;
          if (! isDisposed) {
            disposable |> RxReason.SerialDisposable.set(work());
          };
        },
        int_of_float(delay),
      );
    let timeoutDisposable =
      RxReason.Disposable.create(() => Js.Global.clearTimeout(timeoutId));
    disposable |> RxReason.SerialDisposable.set(timeoutDisposable);
    disposable |> RxReason.SerialDisposable.asDisposable;
  };

  let clockScheduler: RxReason.ClockScheduler.t = {
    getCurrentTime,
    schedule,
    scheduleWithDelay,
  };
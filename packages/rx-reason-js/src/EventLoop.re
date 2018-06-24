[@bs.val]
external setInterval1 : ('a => unit, float, 'a) => Js.Global.intervalId = "";


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

  let executor = ((), continuation, state, f) => {
    let disposable = RxReason.Disposable.empty();
    continuation |> RxReason.SchedulerContinuation.set(disposable);

    Js.Promise.resolve((continuation, state, f, disposable))
    |> Js.Promise.then_(run)
    |> ignore;
  };
  {executor: executor};
};

type relativeTimeExecutorState('state) = {
  mutable interval: RxReason.Disposable.t,
  mutable pending: bool,
  mutable delay: float,
  mutable continuation: RxReason.RelativeTimeSchedulerContinuation.t('state),
  mutable f:
    ('state, RxReason.RelativeTimeSchedulerContinuation.t('state)) => unit,
  mutable state: option('state),
};

let defaultF = (_, _) => ();

let relativeTimeScheduler: RxReason.RelativeTimeSchedulerNew.t = {
  let run = self => {
    self.pending = false;

    let f = self.f;
    self.f = defaultF;

    let continuation = self.continuation;
    self.continuation = RxReason.RelativeTimeSchedulerContinuation.disposed;

    switch (self.state) {
    | Some(state) =>
      self.state = None;
      f(state, continuation);
    | _ => ()
    };
  };

  let shouldRecycleInterval = (delay, self) =>
    ! self.pending && self.delay === delay;

  let scheduleInterval = (delay, self) => {
    let intervalId = setInterval1(run, delay, self);
    RxReason.Disposable.create1(Js.Global.clearInterval, intervalId);
  };

  let factory = () => {
    let self = {
      interval: RxReason.Disposable.disposed,
      pending: false,
      delay: (-1.0),
      continuation: RxReason.RelativeTimeSchedulerContinuation.disposed,
      f: defaultF,
      state: None
    };

    (~delay, continuation, state, f) => {
      if (! shouldRecycleInterval(delay, self)) {
        self.interval |> RxReason.Disposable.dispose;

        let interval = scheduleInterval(delay, self);
        self.interval = interval;

        continuation
        |> RxReason.RelativeTimeSchedulerContinuation.set(interval);
      };

      self.pending = true;
      self.delay = delay;
      self.continuation = continuation;
      self.f = f;
      self.state = Some(state);
    };
  };

  {executor: factory, scheduler: schedulerNew};
};
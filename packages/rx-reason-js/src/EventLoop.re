[@bs.val]
external setInterval1 : ('a => unit, float, 'a) => Js.Global.intervalId = "";

let resolveUnit = Js.Promise.resolve();
/*
let scheduler: RxReason.Scheduler.t = {
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
};*/

type executorState('state) = {
  mutable interval: RxReason.Disposable.t,
  mutable pending: bool,
  mutable delay: float,
  mutable continuation: RxReason.SchedulerContinuation.t('state),
  mutable f:
    ('state, RxReason.SchedulerContinuation.t('state)) => unit,
  mutable state: option('state),
};

let defaultF = (_, _) => ();

let scheduler: RxReason.Scheduler.t = {
  let run = self => {
    self.pending = false;

    let f = self.f;
    self.f = defaultF;

    let continuation = self.continuation;
    self.continuation = RxReason.SchedulerContinuation.disposed;

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
      continuation: RxReason.SchedulerContinuation.disposed,
      f: defaultF,
      state: None,
    };

    (~delay, continuation, state, f) => {
      if (! shouldRecycleInterval(delay, self)) {
        self.interval |> RxReason.Disposable.dispose;

        /** FIXME: Use a Promise if the delay is 0. */

        let interval = scheduleInterval(delay, self);
        self.interval = interval;

        continuation |> RxReason.SchedulerContinuation.set(interval);
      };

      self.pending = true;
      self.delay = delay;
      self.continuation = continuation;
      self.f = f;
      self.state = Some(state);
    };
  };

  {executor: factory, now: Js.Date.now};
};
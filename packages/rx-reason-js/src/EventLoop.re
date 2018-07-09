[@bs.val]
external setInterval1 : ('a => unit, float, 'a) => Js.Global.intervalId = "";

let resolveUnit = Js.Promise.resolve();

type executorState('state) = {
  mutable interval: RxReason.Disposable.t,
  mutable pending: bool,
  mutable delay: float,
  mutable continuation: RxReason.SchedulerContinuation.t('state),
  mutable f: ('state, RxReason.SchedulerContinuation.t('state)) => unit,
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
    ! self.pending && delay !== 0.0 && self.delay === delay;

  let scheduleInterval = (delay, self) => {
    let intervalId = setInterval1(run, delay, self);
    RxReason.Disposable.create1(Js.Global.clearInterval, intervalId);
  };

  let promiseContinuation = self => {
    run(self);
    resolveUnit;
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

        self.interval = (
          if (delay !== 0.0) {
            scheduleInterval(delay, self);
          } else {
            Js.Promise.resolve(self)
            |> Js.Promise.then_(promiseContinuation)
            |> ignore;

            RxReason.Disposable.empty();
          }
        );
      };

      self.pending = true;
      self.delay = delay;
      self.continuation = continuation;
      self.f = f;
      self.state = Some(state);

      self.interval;
    };
  };

  {executor: factory, now: Js.Date.now};
};
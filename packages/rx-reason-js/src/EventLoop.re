[@bs.val]
external setInterval1 : ('a => unit, float, 'a) => Js.Global.intervalId = "";

let resolveUnit = Js.Promise.resolve();

type executorState('state, 'a) = {
  mutable interval: RxReason.Disposable.t,
  mutable pending: bool,
  mutable delay: float,
};

let scheduler: RxReason.Scheduler.t = {
  let run = ((self, continuation, state, f)) => {
    self.pending = false;
    f(state, continuation);
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
    };

    (~delay, continuation, state, f) => {
 
      if (! shouldRecycleInterval(delay, self)) {
        self.interval |> RxReason.Disposable.dispose;

        let ctx = (self, continuation, state, f);

        self.interval = (
          if (delay !== 0.0) {
            scheduleInterval(delay, ctx);
          } else {
            
            Js.Promise.resolve(ctx)
            |> Js.Promise.then_(promiseContinuation)
            |> ignore;

            RxReason.Disposable.empty();
          }
        );
      };

      self.pending = true;
      self.delay = delay;
      self.interval;
    };
  };

  {executor: factory, now: Js.Date.now};
};
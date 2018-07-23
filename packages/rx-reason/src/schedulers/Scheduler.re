module RecursiveResult = {
  type t('state) =
    | Done
    | ContinueAfter(float, 'state);

  let continue = state => ContinueAfter(0.0, state);
  let continueAfter = (~delay, state) => ContinueAfter(delay, state);
  let done_ = Done;
};

type t = {
  now: unit => float,
  scheduleAfter: 'ctx .(~delay: float, 'ctx => unit, 'ctx) => Disposable.t,
  schedulePeriodic: 'ctx .(~delay: float, 'ctx => unit, 'ctx) => Disposable.t,
};

module RecursiveScheduler = {
  type schedulerState('state) = {
    scheduler: t,
    disposable: SerialDisposable.t,
    f: 'state => RecursiveResult.t('state),
    mutable delay: float,
    mutable state: 'state,
  };

  type schedulerState1('state, 'ctx0) = {
    scheduler: t,
    disposable: SerialDisposable.t,
    f: ('ctx0, 'state) => RecursiveResult.t('state),
    mutable delay: float,
    mutable state: 'state,
    ctx0: 'ctx0,
  };

  type schedulerState2('state, 'ctx0, 'ctx1) = {
    scheduler: t,
    disposable: SerialDisposable.t,
    f: ('ctx0, 'ctx1, 'state) => RecursiveResult.t('state),
    mutable delay: float,
    mutable state: 'state,
    ctx0: 'ctx0,
    ctx1: 'ctx1,
  };

  type schedulerState3('state, 'ctx0, 'ctx1, 'ctx2) = {
    scheduler: t,
    disposable: SerialDisposable.t,
    f: ('ctx0, 'ctx1, 'ctx2, 'state) => RecursiveResult.t('state),
    mutable delay: float,
    mutable state: 'state,
    ctx0: 'ctx0,
    ctx1: 'ctx1,
    ctx2: 'ctx2,
  };

  type schedulerState4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3) = {
    scheduler: t,
    disposable: SerialDisposable.t,
    f: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state) => RecursiveResult.t('state),
    mutable delay: float,
    mutable state: 'state,
    ctx0: 'ctx0,
    ctx1: 'ctx1,
    ctx2: 'ctx2,
    ctx3: 'ctx3,
  };

  type schedulerState5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4) = {
    scheduler: t,
    disposable: SerialDisposable.t,
    f:
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state) => RecursiveResult.t('state),
    mutable delay: float,
    mutable state: 'state,
    ctx0: 'ctx0,
    ctx1: 'ctx1,
    ctx2: 'ctx2,
    ctx3: 'ctx3,
    ctx4: 'ctx4,
  };

  type schedulerState6('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5) = {
    scheduler: t,
    disposable: SerialDisposable.t,
    f:
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state) =>
      RecursiveResult.t('state),
    mutable delay: float,
    mutable state: 'state,
    ctx0: 'ctx0,
    ctx1: 'ctx1,
    ctx2: 'ctx2,
    ctx3: 'ctx3,
    ctx4: 'ctx4,
    ctx5: 'ctx5,
  };

  type schedulerState7(
    'state,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    'ctx6,
  ) = {
    scheduler: t,
    disposable: SerialDisposable.t,
    f:
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'state) =>
      RecursiveResult.t('state),
    mutable delay: float,
    mutable state: 'state,
    ctx0: 'ctx0,
    ctx1: 'ctx1,
    ctx2: 'ctx2,
    ctx3: 'ctx3,
    ctx4: 'ctx4,
    ctx5: 'ctx5,
    ctx6: 'ctx6,
  };

  type t('state) =
    | C0(schedulerState('state))
    | C1(schedulerState1('state, 'ctx0)): t('state)
    | C2(schedulerState2('state, 'ctx0, 'ctx1)): t('state)
    | C3(schedulerState3('state, 'ctx0, 'ctx1, 'ctx2)): t('state)
    | C4(schedulerState4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3)): t('state)
    | C5(schedulerState5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4)): t(
                                                                    'state,
                                                                    )
    | C6(schedulerState6('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5)): t(
                                                                    'state,
                                                                    )
    | C7(
          schedulerState7(
            'state,
            'ctx0,
            'ctx1,
            'ctx2,
            'ctx3,
            'ctx4,
            'ctx5,
            'ctx6,
          ),
        ): t('state);

  let getDisposable =
    fun
    | C0({disposable})
    | C1({disposable})
    | C2({disposable})
    | C3({disposable})
    | C4({disposable})
    | C5({disposable})
    | C6({disposable})
    | C7({disposable}) => disposable;

  let setDelayAndState = (delay, state, self) =>
    switch (self) {
    | C0(self) =>
      self.delay = delay;
      self.state = state;
    | C1(self) =>
      self.delay = delay;
      self.state = state;
    | C2(self) =>
      self.delay = delay;
      self.state = state;
    | C3(self) =>
      self.delay = delay;
      self.state = state;
    | C4(self) =>
      self.delay = delay;
      self.state = state;
    | C5(self) =>
      self.delay = delay;
      self.state = state;
    | C6(self) =>
      self.delay = delay;
      self.state = state;
    | C7(self) =>
      self.delay = delay;
      self.state = state;
    };

  let setState = (state, self) =>
    switch (self) {
    | C0(self) => self.state = state
    | C1(self) => self.state = state
    | C2(self) => self.state = state
    | C3(self) => self.state = state
    | C4(self) => self.state = state
    | C5(self) => self.state = state
    | C6(self) => self.state = state
    | C7(self) => self.state = state
    };

  let shouldRun = self =>
    self |> getDisposable |> SerialDisposable.isDisposed |> (!);

  let shouldRecyclePeriodic = oldDelay =>
    fun
    | C0({delay})
    | C1({delay})
    | C2({delay})
    | C3({delay})
    | C4({delay})
    | C5({delay})
    | C6({delay})
    | C7({delay}) => delay !== 0.0 && delay === oldDelay;

  let call =
    fun
    | C0({f, state}) => f(state)
    | C1({f, ctx0, state}) => f(ctx0, state)
    | C2({f, ctx0, ctx1, state}) => f(ctx0, ctx1, state)
    | C3({f, ctx0, ctx1, ctx2, state}) => f(ctx0, ctx1, ctx2, state)
    | C4({f, ctx0, ctx1, ctx2, ctx3, state}) =>
      f(ctx0, ctx1, ctx2, ctx3, state)
    | C5({f, ctx0, ctx1, ctx2, ctx3, ctx4, state}) =>
      f(ctx0, ctx1, ctx2, ctx3, ctx4, state)
    | C6({f, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state}) =>
      f(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state)
    | C7({f, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state}) =>
      f(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state);

  let rec work = self => {
    let shouldRun = shouldRun(self);
    let disposable = self |> getDisposable;

    if (shouldRun) {
      let result = call(self);
      switch (result) {
      | RecursiveResult.ContinueAfter(delay, state)
          when ! shouldRecyclePeriodic(delay, self) =>
        disposable |> SerialDisposable.getInnerDisposable |> Disposable.dispose;
        self |> setDelayAndState(delay, state);
        schedule(self);
      | RecursiveResult.ContinueAfter(_, state) => self |> setState(state)
      | RecursiveResult.Done => disposable |> SerialDisposable.dispose
      };
    };
  }
  and schedule = self =>
    switch (self) {
    | C0({delay, scheduler, disposable})
    | C1({delay, scheduler, disposable})
    | C2({delay, scheduler, disposable})
    | C3({delay, scheduler, disposable})
    | C4({delay, scheduler, disposable})
    | C5({delay, scheduler, disposable})
    | C6({delay, scheduler, disposable})
    | C7({delay, scheduler, disposable}) =>
      if (delay > 0.0) {
        let innerDisposable = scheduler.schedulePeriodic(~delay, work, self);
        disposable |> SerialDisposable.setInnerDisposable(innerDisposable);
      } else {
        let innerDisposable = scheduler.scheduleAfter(~delay, work, self);
        disposable |> SerialDisposable.setInnerDisposable(innerDisposable);
      }
    };
};

let now = scheduler => scheduler.now();

let scheduleRecursive = (~delay, f, state, scheduler) => {
  let disposable = SerialDisposable.create();
  RecursiveScheduler.(
    schedule(C0({scheduler, disposable, f, delay, state}))
  );
  disposable |> SerialDisposable.asDisposable;
};

let scheduleRecursive1 = (~delay, f, state, ctx0, scheduler) => {
  let disposable = SerialDisposable.create();
  RecursiveScheduler.(
    schedule(C1({scheduler, disposable, f, delay, state, ctx0}))
  );
  disposable |> SerialDisposable.asDisposable;
};

let scheduleRecursive2 = (~delay, f, state, ctx0, ctx1, scheduler) => {
  let disposable = SerialDisposable.create();
  RecursiveScheduler.(
    schedule(C2({scheduler, disposable, f, delay, state, ctx0, ctx1}))
  );
  disposable |> SerialDisposable.asDisposable;
};

let scheduleRecursive3 = (~delay, f, state, ctx0, ctx1, ctx2, scheduler) => {
  let disposable = SerialDisposable.create();
  RecursiveScheduler.(
    schedule(C3({scheduler, disposable, f, delay, state, ctx0, ctx1, ctx2}))
  );
  disposable |> SerialDisposable.asDisposable;
};

let scheduleRecursive4 = (~delay, f, state, ctx0, ctx1, ctx2, ctx3, scheduler) => {
  let disposable = SerialDisposable.create();
  RecursiveScheduler.(
    schedule(
      C4({scheduler, disposable, f, delay, state, ctx0, ctx1, ctx2, ctx3}),
    )
  );
  disposable |> SerialDisposable.asDisposable;
};

let scheduleRecursive5 =
    (~delay, f, state, ctx0, ctx1, ctx2, ctx3, ctx4, scheduler) => {
  let disposable = SerialDisposable.create();
  RecursiveScheduler.(
    schedule(
      C5({
        scheduler,
        disposable,
        f,
        delay,
        state,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
      }),
    )
  );
  disposable |> SerialDisposable.asDisposable;
};

let scheduleRecursive6 =
    (~delay, f, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, scheduler) => {
  let disposable = SerialDisposable.create();
  RecursiveScheduler.(
    schedule(
      C6({
        scheduler,
        disposable,
        f,
        delay,
        state,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
      }),
    )
  );
  disposable |> SerialDisposable.asDisposable;
};

let scheduleRecursive7 =
    (~delay, f, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, scheduler) => {
  let disposable = SerialDisposable.create();
  RecursiveScheduler.(
    schedule(
      C7({
        scheduler,
        disposable,
        f,
        delay,
        state,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
      }),
    )
  );
  disposable |> SerialDisposable.asDisposable;
};

let scheduleAfter = (~delay, f, state, {scheduleAfter}) => {
  let disposable = SerialDisposable.create();

  let work = () =>
    if (! SerialDisposable.isDisposed(disposable)) {
      f(state);
    };

  disposable
  |> SerialDisposable.setInnerDisposable(scheduleAfter(~delay, work, ()));
  disposable |> SerialDisposable.asDisposable;
};

let scheduleAfter1 = (~delay, f, state, ctx0, {scheduleAfter}) => {
  let disposable = SerialDisposable.create();

  let work = () =>
    if (! SerialDisposable.isDisposed(disposable)) {
      f(ctx0, state);
    };

  disposable
  |> SerialDisposable.setInnerDisposable(scheduleAfter(~delay, work, ()));
  disposable |> SerialDisposable.asDisposable;
};

let scheduleAfter2 = (~delay, f, state, ctx0, ctx1, {scheduleAfter}) => {
  let disposable = SerialDisposable.create();

  let work = () =>
    if (! SerialDisposable.isDisposed(disposable)) {
      f(ctx0, ctx1, state);
    };

  disposable
  |> SerialDisposable.setInnerDisposable(scheduleAfter(~delay, work, ()));
  disposable |> SerialDisposable.asDisposable;
};

let scheduleAfter3 = (~delay, f, state, ctx0, ctx1, ctx2, {scheduleAfter}) => {
  let disposable = SerialDisposable.create();

  let work = () =>
    if (! SerialDisposable.isDisposed(disposable)) {
      f(ctx0, ctx1, ctx2, state);
    };

  disposable
  |> SerialDisposable.setInnerDisposable(scheduleAfter(~delay, work, ()));
  disposable |> SerialDisposable.asDisposable;
};

let scheduleAfter4 =
    (~delay, f, state, ctx0, ctx1, ctx2, ctx3, {scheduleAfter}) => {
  let disposable = SerialDisposable.create();

  let work = () =>
    if (! SerialDisposable.isDisposed(disposable)) {
      f(ctx0, ctx1, ctx2, ctx3, state);
    };

  disposable
  |> SerialDisposable.setInnerDisposable(scheduleAfter(~delay, work, ()));
  disposable |> SerialDisposable.asDisposable;
};

let scheduleAfter5 =
    (~delay, f, state, ctx0, ctx1, ctx2, ctx3, ctx4, {scheduleAfter}) => {
  let disposable = SerialDisposable.create();

  let work = () =>
    if (! SerialDisposable.isDisposed(disposable)) {
      f(ctx0, ctx1, ctx2, ctx3, ctx4, state);
    };

  disposable
  |> SerialDisposable.setInnerDisposable(scheduleAfter(~delay, work, ()));
  disposable |> SerialDisposable.asDisposable;
};

let scheduleAfter6 =
    (~delay, f, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, {scheduleAfter}) => {
  let disposable = SerialDisposable.create();

  let work = () =>
    if (! SerialDisposable.isDisposed(disposable)) {
      f(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state);
    };

  disposable
  |> SerialDisposable.setInnerDisposable(scheduleAfter(~delay, work, ()));
  disposable |> SerialDisposable.asDisposable;
};

let scheduleAfter7 =
    (
      ~delay,
      f,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      {scheduleAfter},
    ) => {
  let disposable = SerialDisposable.create();

  let work = () =>
    if (! SerialDisposable.isDisposed(disposable)) {
      f(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state);
    };

  disposable
  |> SerialDisposable.setInnerDisposable(scheduleAfter(~delay, work, ()));
  disposable |> SerialDisposable.asDisposable;
};

let schedule = (f, state, scheduler) =>
  scheduleAfter(~delay=0.0, f, state, scheduler);

let schedule1 = (f, state, ctx0, scheduler) =>
  scheduleAfter1(~delay=0.0, f, state, ctx0, scheduler);

let schedule2 = (f, state, ctx0, ctx1, scheduler) =>
  scheduleAfter2(~delay=0.0, f, state, ctx0, ctx1, scheduler);

let schedule3 = (f, state, ctx0, ctx1, ctx2, scheduler) =>
  scheduleAfter3(~delay=0.0, f, state, ctx0, ctx1, ctx2, scheduler);

let schedule4 = (f, state, ctx0, ctx1, ctx2, ctx3, scheduler) =>
  scheduleAfter4(~delay=0.0, f, state, ctx0, ctx1, ctx2, ctx3, scheduler);

let schedule5 = (f, state, ctx0, ctx1, ctx2, ctx3, ctx4, scheduler) =>
  scheduleAfter5(
    ~delay=0.0,
    f,
    state,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    scheduler,
  );

let schedule6 = (f, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, scheduler) =>
  scheduleAfter6(
    ~delay=0.0,
    f,
    state,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    scheduler,
  );

let schedule7 =
    (f, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, scheduler) =>
  scheduleAfter7(
    ~delay=0.0,
    f,
    state,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
    scheduler,
  );

let computeDelay = (time, scheduler) => {
  let now = scheduler.now();
  time -. now;
};

let scheduleBy = (~time, f, state, scheduler) => {
  let delay = scheduler |> computeDelay(time);
  if (! (delay < 0.0)) {
    scheduleAfter(~delay, f, state, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy1 = (~time, f, state, ctx0, scheduler) => {
  let delay = scheduler |> computeDelay(time);
  if (! (delay < 0.0)) {
    scheduleAfter1(~delay, f, state, ctx0, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy2 = (~time, f, state, ctx0, ctx1, scheduler) => {
  let delay = scheduler |> computeDelay(time);
  if (! (delay < 0.0)) {
    scheduleAfter2(~delay, f, state, ctx0, ctx1, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy3 = (~time, f, state, ctx0, ctx1, ctx2, scheduler) => {
  let delay = scheduler |> computeDelay(time);
  if (! (delay < 0.0)) {
    scheduleAfter3(~delay, f, state, ctx0, ctx1, ctx2, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy4 = (~time, f, state, ctx0, ctx1, ctx2, ctx3, scheduler) => {
  let delay = scheduler |> computeDelay(time);
  if (! (delay < 0.0)) {
    scheduleAfter4(~delay, f, state, ctx0, ctx1, ctx2, ctx3, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy5 = (~time, f, state, ctx0, ctx1, ctx2, ctx3, ctx4, scheduler) => {
  let delay = scheduler |> computeDelay(time);
  if (! (delay < 0.0)) {
    scheduleAfter5(~delay, f, state, ctx0, ctx1, ctx2, ctx3, ctx4, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy6 =
    (~time, f, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, scheduler) => {
  let delay = scheduler |> computeDelay(time);
  if (! (delay < 0.0)) {
    scheduleAfter6(
      ~delay,
      f,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      scheduler,
    );
  } else {
    Disposable.disposed;
  };
};

let scheduleBy7 =
    (~time, f, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, scheduler) => {
  let delay = scheduler |> computeDelay(time);
  if (! (delay < 0.0)) {
    scheduleAfter7(
      ~delay,
      f,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      scheduler,
    );
  } else {
    Disposable.disposed;
  };
};
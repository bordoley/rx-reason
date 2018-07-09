type t = {
  executor: 'a 'state .unit => SchedulerExecutor.t('state, 'a),
  now: unit => float,
};

module SchedulerContinuation = {
  type t('state) =
    | C0(
        SchedulerExecutor.t('state, t('state)),
        SerialDisposable.t,
        ScheduledWork.t('state),
      )
    | C1(
          SchedulerExecutor.t('state, t('state)),
          SerialDisposable.t,
          ScheduledWork.t1('ctx0, 'state),
          'ctx0,
        ): t('state)
    | C2(
          SchedulerExecutor.t('state, t('state)),
          SerialDisposable.t,
          ScheduledWork.t2('ctx0, 'ctx1, 'state),
          'ctx0,
          'ctx1,
        ): t('state)
    | C3(
          SchedulerExecutor.t('state, t('state)),
          SerialDisposable.t,
          ScheduledWork.t3('ctx0, 'ctx1, 'ctx2, 'state),
          'ctx0,
          'ctx1,
          'ctx2,
        ): t('state)
    | C4(
          SchedulerExecutor.t('state, t('state)),
          SerialDisposable.t,
          ScheduledWork.t4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state),
          'ctx0,
          'ctx1,
          'ctx2,
          'ctx3,
        ): t('state)
    | C5(
          SchedulerExecutor.t('state, t('state)),
          SerialDisposable.t,
          ScheduledWork.t5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state),
          'ctx0,
          'ctx1,
          'ctx2,
          'ctx3,
          'ctx4,
        ): t('state)
    | C6(
          SchedulerExecutor.t('state, t('state)),
          SerialDisposable.t,
          ScheduledWork.t6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state),
          'ctx0,
          'ctx1,
          'ctx2,
          'ctx3,
          'ctx4,
          'ctx5,
        ): t('state)
    | C7(
          SchedulerExecutor.t('state, t('state)),
          SerialDisposable.t,
          ScheduledWork.t7(
            'ctx0,
            'ctx1,
            'ctx2,
            'ctx3,
            'ctx4,
            'ctx5,
            'ctx6,
            'state,
          ),
          'ctx0,
          'ctx1,
          'ctx2,
          'ctx3,
          'ctx4,
          'ctx5,
          'ctx6,
        ): t('state);
  
  let asSerialDisposable =
    fun
    | C0(_, disposable, _)
    | C1(_, disposable, _, _)
    | C2(_, disposable, _, _, _)
    | C3(_, disposable, _, _, _, _)
    | C4(_, disposable, _, _, _, _, _)
    | C5(_, disposable, _, _, _, _, _, _)
    | C6(_, disposable, _, _, _, _, _, _, _)
    | C7(_, disposable, _, _, _, _, _, _, _, _) => disposable;
  
  let asDisposable = continuation =>
    continuation |> asSerialDisposable |> SerialDisposable.asDisposable;
  
  let create = (executor, work) =>
    C0(executor, SerialDisposable.create(), work);
  
  let create1 = (executor, work, ctx0) =>
    C1(executor, SerialDisposable.create(), work, ctx0);
  
  let create2 = (executor, work, ctx0, ctx1) =>
    C2(executor, SerialDisposable.create(), work, ctx0, ctx1);
  
  let create3 = (executor, work, ctx0, ctx1, ctx2) =>
    C3(executor, SerialDisposable.create(), work, ctx0, ctx1, ctx2);
  
  let create4 = (executor, work, ctx0, ctx1, ctx2, ctx3) =>
    C4(executor, SerialDisposable.create(), work, ctx0, ctx1, ctx2, ctx3);
  
  let create5 = (executor, work, ctx0, ctx1, ctx2, ctx3, ctx4) =>
    C5(executor, SerialDisposable.create(), work, ctx0, ctx1, ctx2, ctx3, ctx4);
  
  let create6 = (executor, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) =>
    C6(
      executor,
      SerialDisposable.create(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
    );
  
  let create7 = (executor, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6) =>
    C7(
      executor,
      SerialDisposable.create(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
    );
  
  let dispose = continuation =>
    continuation |> asDisposable |> Disposable.dispose;
  
  let isDisposed = continuation =>
    continuation |> asDisposable |> Disposable.isDisposed;
  
  let rec continueAfter = {
    let getExecutor =
      fun
      | C0(executor, _, _)
      | C1(executor, _, _, _)
      | C2(executor, _, _, _, _)
      | C3(executor, _, _, _, _, _)
      | C4(executor, _, _, _, _, _, _)
      | C5(executor, _, _, _, _, _, _, _)
      | C6(executor, _, _, _, _, _, _, _, _)
      | C7(executor, _, _, _, _, _, _, _, _, _) => executor;
  
    let doWork = (state, continuation) =>
      switch (continuation) {
      | C0(_, _, work) => work(state)
      | C1(_, _, work, ctx0) => work(ctx0, state)
      | C2(_, _, work, ctx0, ctx1) => work(ctx0, ctx1, state)
      | C3(_, _, work, ctx0, ctx1, ctx2) => work(ctx0, ctx1, ctx2, state)
      | C4(_, _, work, ctx0, ctx1, ctx2, ctx3) =>
        work(ctx0, ctx1, ctx2, ctx3, state)
      | C5(_, _, work, ctx0, ctx1, ctx2, ctx3, ctx4) =>
        work(ctx0, ctx1, ctx2, ctx3, ctx4, state)
      | C6(_, _, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) =>
        work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state)
      | C7(_, _, work, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6) =>
        work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state)
      };
  
    let execute = (state, continuation) =>
      if (! isDisposed(continuation)) {
        let result = doWork(state, continuation);
        switch (result) {
        | Done => continuation |> dispose
        | Continue(state) => continuation |> continueAfter(~delay=0.0, state)
        | ContinueAfter(delay, state) =>
          continuation |> continueAfter(~delay, state)
        };
      };
  
    (~delay, state, continuation) => {
      let innerDisposableActive =
        continuation
        |> asSerialDisposable
        |> SerialDisposable.getInnerDisposable
        |> Disposable.isDisposed
        |> (!);
  
      if (innerDisposableActive) {
        /** FIXME: define a real exception type here */
        failwith("continue called multiple times");
      };
  
      let executor = continuation |> getExecutor;
      let innerDisposable = executor(~delay, continuation, state, execute);
  
      continuation
      |> asSerialDisposable
      |> SerialDisposable.setInnerDisposable(innerDisposable);
    };
  };
};

let now = ({now}) => now();

let scheduleAfter = (~delay, work, state, {executor}) => {
  let continuation = SchedulerContinuation.create(executor(), work);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter1 = (~delay, work, state, ctx0, {executor}) => {
  let continuation = SchedulerContinuation.create1(executor(), work, ctx0);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter2 = (~delay, work, state, ctx0, ctx1, {executor}) => {
  let continuation =
    SchedulerContinuation.create2(executor(), work, ctx0, ctx1);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter3 = (~delay, work, state, ctx0, ctx1, ctx2, {executor}) => {
  let continuation =
    SchedulerContinuation.create3(executor(), work, ctx0, ctx1, ctx2);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter4 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, {executor}) => {
  let continuation =
    SchedulerContinuation.create4(executor(), work, ctx0, ctx1, ctx2, ctx3);
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter5 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, {executor}) => {
  let continuation =
    SchedulerContinuation.create5(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
    );
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter6 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, {executor}) => {
  let continuation =
    SchedulerContinuation.create6(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
    );
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let scheduleAfter7 =
    (
      ~delay,
      work,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      {executor},
    ) => {
  let continuation =
    SchedulerContinuation.create7(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
    );
  continuation |> SchedulerContinuation.continueAfter(~delay, state);
  continuation |> SchedulerContinuation.asDisposable;
};

let schedule = (work, state, scheduler) =>
  scheduleAfter(~delay=0.0, work, state, scheduler);

let schedule1 = (work, state, ctx0, scheduler) =>
  scheduleAfter1(~delay=0.0, work, state, ctx0, scheduler);

let schedule2 = (work, state, ctx0, ctx1, scheduler) =>
  scheduleAfter2(~delay=0.0, work, state, ctx0, ctx1, scheduler);

let schedule3 = (work, state, ctx0, ctx1, ctx2, scheduler) =>
  scheduleAfter3(~delay=0.0, work, state, ctx0, ctx1, ctx2, scheduler);

let schedule4 = (work, state, ctx0, ctx1, ctx2, ctx3, scheduler) =>
  scheduleAfter4(~delay=0.0, work, state, ctx0, ctx1, ctx2, ctx3, scheduler);

let schedule5 = (work, state, ctx0, ctx1, ctx2, ctx3, ctx4, scheduler) =>
  scheduleAfter5(
    ~delay=0.0,
    work,
    state,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    scheduler,
  );

let schedule6 = (work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, scheduler) =>
  scheduleAfter6(
    ~delay=0.0,
    work,
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
    (work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, scheduler) =>
  scheduleAfter7(
    ~delay=0.0,
    work,
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

let scheduleBy = (~time, work, state, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = state => {
      work(state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter(~delay, work, state, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy1 = (~time, work, state, ctx0, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, state) => {
      work(ctx0, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter1(~delay=0.0, work, state, ctx0, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy2 = (~time, work, state, ctx0, ctx1, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, state) => {
      work(ctx0, ctx1, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter2(~delay=0.0, work, state, ctx0, ctx1, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy3 = (~time, work, state, ctx0, ctx1, ctx2, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, state) => {
      work(ctx0, ctx1, ctx2, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter3(~delay=0.0, work, state, ctx0, ctx1, ctx2, scheduler);
  } else {
    Disposable.disposed;
  };
};

let scheduleBy4 = (~time, work, state, ctx0, ctx1, ctx2, ctx3, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, state) => {
      work(ctx0, ctx1, ctx2, ctx3, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter4(
      ~delay=0.0,
      work,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      scheduler,
    );
  } else {
    Disposable.disposed;
  };
};

let scheduleBy5 =
    (~time, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, ctx4, state) => {
      work(ctx0, ctx1, ctx2, ctx3, ctx4, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter5(
      ~delay=0.0,
      work,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      scheduler,
    );
  } else {
    Disposable.disposed;
  };
};

let scheduleBy6 =
    (~time, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state) => {
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter6(
      ~delay=0.0,
      work,
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
    (~time, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, scheduler) => {
  let now = scheduler |> now;
  let delay = time -. now;
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state) => {
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state);
      ScheduledWork.Result.Done;
    };
    scheduleAfter7(
      ~delay=0.0,
      work,
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
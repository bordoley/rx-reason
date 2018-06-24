type t = {
  executor:
    'state .
    unit => DelayableSchedulerContinuation.Executor.t('state),

  now: unit => float,
  scheduler: SchedulerNew.t,
};

type relativeTimeScheduler = t;

module type S = {
  type t;

  let asRelativeTimeScheduler: t => relativeTimeScheduler;

  let now: unit => float;

  let scheduleAfter:
    (
      ~delay: float,
      DelayableSchedulerContinuation.Work.t('state),
      'state,
      t
    ) =>
    Disposable.t;

  let scheduleAfter1:
    (
      ~delay: float,
      DelayableSchedulerContinuation.Work.t1('state, 'ctx0),
      'state,
      'ctx0,
      t
    ) =>
    Disposable.t;

  let scheduleAfter2:
    (
      ~delay: float,
      DelayableSchedulerContinuation.Work.t2('state, 'ctx0, 'ctx1),
      'state,
      'ctx0,
      'ctx1,
      t
    ) =>
    Disposable.t;

  let scheduleAfter3:
    (
      ~delay: float,
      DelayableSchedulerContinuation.Work.t3('state, 'ctx0, 'ctx1, 'ctx2),
      'state,
      'ctx0,
      'ctx1,
      'ctx2,
      t
    ) =>
    Disposable.t;

  let scheduleAfter4:
    (
      ~delay: float,
      DelayableSchedulerContinuation.Work.t4(
        'state,
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
      ),
      'state,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      t
    ) =>
    Disposable.t;

  let scheduleAfter5:
    (
      ~delay: float,
      DelayableSchedulerContinuation.Work.t5(
        'state,
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
      ),
      'state,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      t
    ) =>
    Disposable.t;

  let scheduleAfter6:
    (
      ~delay: float,
      DelayableSchedulerContinuation.Work.t6(
        'state,
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
        'ctx5,
      ),
      'state,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      t
    ) =>
    Disposable.t;

  let scheduleAfter7:
    (
      ~delay: float,
      DelayableSchedulerContinuation.Work.t7(
        'state,
        'ctx0,
        'ctx1,
        'ctx2,
        'ctx3,
        'ctx4,
        'ctx5,
        'ctx6,
      ),
      'state,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      'ctx6,
      t
    ) =>
    Disposable.t;
};

let asRelativeTimeScheduler = Functions.identity;

let scheduleAfter = (~delay, work, state, {executor}) => {
  let continuation =
    DelayableSchedulerContinuation.create(executor(), work);
  continuation |> DelayableSchedulerContinuation.continue(~delay, state);
  continuation |> DelayableSchedulerContinuation.asDisposable;
};

let scheduleAfter1 = (~delay, work, state, ctx0, {executor}) => {
  let continuation =
    DelayableSchedulerContinuation.create1(executor(), work, ctx0);
  continuation |> DelayableSchedulerContinuation.continue(~delay, state);
  continuation |> DelayableSchedulerContinuation.asDisposable;
};

let scheduleAfter2 = (~delay, work, state, ctx0, ctx1, {executor}) => {
  let continuation =
    DelayableSchedulerContinuation.create2(executor(), work, ctx0, ctx1);
  continuation |> DelayableSchedulerContinuation.continue(~delay, state);
  continuation |> DelayableSchedulerContinuation.asDisposable;
};

let scheduleAfter3 = (~delay, work, state, ctx0, ctx1, ctx2, {executor}) => {
  let continuation =
    DelayableSchedulerContinuation.create3(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
    );
  continuation |> DelayableSchedulerContinuation.continue(~delay, state);
  continuation |> DelayableSchedulerContinuation.asDisposable;
};

let scheduleAfter4 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, {executor}) => {
  let continuation =
    DelayableSchedulerContinuation.create4(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
    );
  continuation |> DelayableSchedulerContinuation.continue(~delay, state);
  continuation |> DelayableSchedulerContinuation.asDisposable;
};

let scheduleAfter5 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, {executor}) => {
  let continuation =
    DelayableSchedulerContinuation.create5(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
    );
  continuation |> DelayableSchedulerContinuation.continue(~delay, state);
  continuation |> DelayableSchedulerContinuation.asDisposable;
};

let scheduleAfter6 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, {executor}) => {
  let continuation =
    DelayableSchedulerContinuation.create6(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
    );
  continuation |> DelayableSchedulerContinuation.continue(~delay, state);
  continuation |> DelayableSchedulerContinuation.asDisposable;
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
    DelayableSchedulerContinuation.create7(
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
  continuation |> DelayableSchedulerContinuation.continue(~delay, state);
  continuation |> DelayableSchedulerContinuation.asDisposable;
};

let scheduleBy = (~time, work, state, {executor, now}) => {
  let delay = time -. now();
  if (! (delay < 0.0)) {
    let work = (_, state) => work(state);
    let continuation =
      DelayableSchedulerContinuation.create(executor(), work);
    continuation |> DelayableSchedulerContinuation.continue(~delay, state);
    continuation |> DelayableSchedulerContinuation.asDisposable;
  } else {
    Disposable.disposed;
  };
};

let scheduleBy1 = (~time, work, state, ctx0, {executor, now}) => {
  let delay = time -. now();
  if (! (delay < 0.0)) {
    let work = (ctx0, _, state) => work(ctx0, state);
    let continuation =
      DelayableSchedulerContinuation.create1(executor(), work, ctx0);
    continuation |> DelayableSchedulerContinuation.continue(~delay, state);
    continuation |> DelayableSchedulerContinuation.asDisposable;
  } else {
    Disposable.disposed;
  };
};

let scheduleBy2 = (~time, work, state, ctx0, ctx1, {executor, now}) => {
  let delay = time -. now();
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, _, state) => work(ctx0, ctx1, state);
    let continuation =
      DelayableSchedulerContinuation.create2(executor(), work, ctx0, ctx1);
    continuation |> DelayableSchedulerContinuation.continue(~delay, state);
    continuation |> DelayableSchedulerContinuation.asDisposable;
  } else {
    Disposable.disposed;
  };
};

let scheduleBy3 = (~time, work, state, ctx0, ctx1, ctx2, {executor, now}) => {
  let delay = time -. now();
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, _, state) => work(ctx0, ctx1, ctx2, state);
    let continuation =
      DelayableSchedulerContinuation.create3(
        executor(),
        work,
        ctx0,
        ctx1,
        ctx2,
      );
    continuation |> DelayableSchedulerContinuation.continue(~delay, state);
    continuation |> DelayableSchedulerContinuation.asDisposable;
  } else {
    Disposable.disposed;
  };
};

let scheduleBy4 =
    (~time, work, state, ctx0, ctx1, ctx2, ctx3, {executor, now}) => {
  let delay = time -. now();
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, _, state) =>
      work(ctx0, ctx1, ctx2, ctx3, state);
    let continuation =
      DelayableSchedulerContinuation.create4(
        executor(),
        work,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
      );
    continuation |> DelayableSchedulerContinuation.continue(~delay, state);
    continuation |> DelayableSchedulerContinuation.asDisposable;
  } else {
    Disposable.disposed;
  };
};

let scheduleBy5 =
    (~time, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, {executor, now}) => {
  let delay = time -. now();
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, ctx4, _, state) =>
      work(ctx0, ctx1, ctx2, ctx3, ctx4, state);
    let continuation =
      DelayableSchedulerContinuation.create5(
        executor(),
        work,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
      );
    continuation |> DelayableSchedulerContinuation.continue(~delay, state);
    continuation |> DelayableSchedulerContinuation.asDisposable;
  } else {
    Disposable.disposed;
  };
};

let scheduleBy6 =
    (~time, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, {executor, now}) => {
  let delay = time -. now();
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, _, state) =>
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, state);
    let continuation =
      DelayableSchedulerContinuation.create6(
        executor(),
        work,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
      );
    continuation |> DelayableSchedulerContinuation.continue(~delay, state);
    continuation |> DelayableSchedulerContinuation.asDisposable;
  } else {
    Disposable.disposed;
  };
};

let scheduleBy7 =
    (
      ~time,
      work,
      state,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      {executor, now},
    ) => {
  let delay = time -. now();
  if (! (delay < 0.0)) {
    let work = (ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, _, state) =>
      work(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, state);
    let continuation =
      DelayableSchedulerContinuation.create7(
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
    continuation |> DelayableSchedulerContinuation.continue(~delay, state);
    continuation |> DelayableSchedulerContinuation.asDisposable;
  } else {
    Disposable.disposed;
  };
};
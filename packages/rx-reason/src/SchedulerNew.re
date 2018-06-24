type t = {
  executor: 'state .unit => SchedulerContinuation.Executor.t('state),
};

type scheduler = t;

module type S = {
  type t;

  let asScheduler: t => scheduler;

  let schedule:
    (SchedulerContinuation.Work.t('state), 'state, t) => Disposable.t;

  let schedule1:
    (SchedulerContinuation.Work.t1('state, 'ctx0), 'state, 'ctx0, t) =>
    Disposable.t;

  let schedule2:
    (
      SchedulerContinuation.Work.t2('state, 'ctx0, 'ctx1),
      'state,
      'ctx0,
      'ctx1,
      t
    ) =>
    Disposable.t;

  let schedule3:
    (
      SchedulerContinuation.Work.t3('state, 'ctx0, 'ctx1, 'ctx2),
      'state,
      'ctx0,
      'ctx1,
      'ctx2,
      t
    ) =>
    Disposable.t;

  let schedule4:
    (
      SchedulerContinuation.Work.t4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3),
      'state,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      t
    ) =>
    Disposable.t;

  let schedule5:
    (
      SchedulerContinuation.Work.t5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4),
      'state,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      t
    ) =>
    Disposable.t;

  let schedule6:
    (
      SchedulerContinuation.Work.t6(
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

  let schedule7:
    (
      SchedulerContinuation.Work.t7(
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

let asScheduler = Functions.identity;

let schedule = (work, state, {executor}) => {
  let continuation = SchedulerContinuation.create(executor(), work);
  continuation |> SchedulerContinuation.continue(state);
  continuation |> SchedulerContinuation.asDisposable;
};

let schedule1 = (work, state, ctx0, {executor}) => {
  let continuation = SchedulerContinuation.create1(executor(), work, ctx0);
  continuation |> SchedulerContinuation.continue(state);
  continuation |> SchedulerContinuation.asDisposable;
};

let schedule2 = (work, state, ctx0, ctx1, {executor}) => {
  let continuation =
    SchedulerContinuation.create2(executor(), work, ctx0, ctx1);
  continuation |> SchedulerContinuation.continue(state);
  continuation |> SchedulerContinuation.asDisposable;
};

let schedule3 = (work, state, ctx0, ctx1, ctx2, {executor}) => {
  let continuation =
    SchedulerContinuation.create3(executor(), work, ctx0, ctx1, ctx2);
  continuation |> SchedulerContinuation.continue(state);
  continuation |> SchedulerContinuation.asDisposable;
};

let schedule4 = (work, state, ctx0, ctx1, ctx2, ctx3, {executor}) => {
  let continuation =
    SchedulerContinuation.create4(executor(), work, ctx0, ctx1, ctx2, ctx3);
  continuation |> SchedulerContinuation.continue(state);
  continuation |> SchedulerContinuation.asDisposable;
};

let schedule5 = (work, state, ctx0, ctx1, ctx2, ctx3, ctx4, {executor}) => {
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
  continuation |> SchedulerContinuation.continue(state);
  continuation |> SchedulerContinuation.asDisposable;
};

let schedule6 = (work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, {executor}) => {
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
  continuation |> SchedulerContinuation.continue(state);
  continuation |> SchedulerContinuation.asDisposable;
};

let schedule7 =
    (work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, {executor}) => {
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
  continuation |> SchedulerContinuation.continue(state);
  continuation |> SchedulerContinuation.asDisposable;
};

let immediate: t = {
  let immediateExecutor = (continuation, state, f) => f(state, continuation);

  {executor: () => immediateExecutor};
};
type t = {
  executor: 'state . unit => RelativeTimeSchedulerContinuation.Executor.t('state),
  scheduler: SchedulerNew.t,
};

type relativeTimeScheduler = t;

module type S = {
  type t;

  let asRelativeTimeScheduler: t => relativeTimeScheduler;

  let scheduleAfter:
    (~delay: float, RelativeTimeSchedulerContinuation.Work.t('state), 'state, t) => Disposable.t;

  let scheduleAfter1:
    (~delay: float, RelativeTimeSchedulerContinuation.Work.t1('state, 'ctx0), 'state, 'ctx0, t) =>
    Disposable.t;

  let scheduleAfter2:
    (
      ~delay: float, 
      RelativeTimeSchedulerContinuation.Work.t2('state, 'ctx0, 'ctx1),
      'state,
      'ctx0,
      'ctx1,
      t
    ) =>
    Disposable.t;

  let scheduleAfter3:
    (
      ~delay: float, 
      RelativeTimeSchedulerContinuation.Work.t3('state, 'ctx0, 'ctx1, 'ctx2),
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
      RelativeTimeSchedulerContinuation.Work.t4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3),
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
      RelativeTimeSchedulerContinuation.Work.t5(
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
      RelativeTimeSchedulerContinuation.Work.t6(
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
      RelativeTimeSchedulerContinuation.Work.t7(
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
  let continuation = RelativeTimeSchedulerContinuation.create(executor(), work);
  continuation |> RelativeTimeSchedulerContinuation.continue(~delay, state);
  continuation |> RelativeTimeSchedulerContinuation.asDisposable;
};

let scheduleAfter1 = (~delay, work, state, ctx0, {executor}) => {
  let continuation = RelativeTimeSchedulerContinuation.create1(executor(), work, ctx0);
  continuation |> RelativeTimeSchedulerContinuation.continue(~delay, state);
  continuation |> RelativeTimeSchedulerContinuation.asDisposable;
};

let scheduleAfter2 = (~delay, work, state, ctx0, ctx1, {executor}) => {
  let continuation =
    RelativeTimeSchedulerContinuation.create2(executor(), work, ctx0, ctx1);
  continuation |> RelativeTimeSchedulerContinuation.continue(~delay, state);
  continuation |> RelativeTimeSchedulerContinuation.asDisposable;
};

let scheduleAfter3 = (~delay, work, state, ctx0, ctx1, ctx2, {executor}) => {
  let continuation =
    RelativeTimeSchedulerContinuation.create3(executor(), work, ctx0, ctx1, ctx2);
  continuation |> RelativeTimeSchedulerContinuation.continue(~delay, state);
  continuation |> RelativeTimeSchedulerContinuation.asDisposable;
};

let scheduleAfter4 = (~delay, work, state, ctx0, ctx1, ctx2, ctx3, {executor}) => {
  let continuation =
    RelativeTimeSchedulerContinuation.create4(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
    );
  continuation |> RelativeTimeSchedulerContinuation.continue(~delay, state);
  continuation |> RelativeTimeSchedulerContinuation.asDisposable;
};

let scheduleAfter5 = (~delay, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, {executor}) => {
  let continuation =
    RelativeTimeSchedulerContinuation.create5(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
    );
  continuation |> RelativeTimeSchedulerContinuation.continue(~delay, state);
  continuation |> RelativeTimeSchedulerContinuation.asDisposable;
};

let scheduleAfter6 = (~delay, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, {executor}) => {
  let continuation =
    RelativeTimeSchedulerContinuation.create6(
      executor(),
      work,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
    );
  continuation |> RelativeTimeSchedulerContinuation.continue(~delay, state);
  continuation |> RelativeTimeSchedulerContinuation.asDisposable;
};

let scheduleAfter7 =
    (~delay, work, state, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, {executor}) => {
  let continuation =
    RelativeTimeSchedulerContinuation.create7(
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
  continuation |> RelativeTimeSchedulerContinuation.continue(~delay, state);
  continuation |> RelativeTimeSchedulerContinuation.asDisposable;
};
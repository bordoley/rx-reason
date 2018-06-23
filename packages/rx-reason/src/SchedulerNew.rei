type t = {executor: 'state .SchedulerContinuation.Executor.t('state)};

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

include S with type t := t;

let immediate: t;
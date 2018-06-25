type t = {
  executor: 'state .unit => SchedulerContinuation.Executor.t('state),
  now: unit => float,
};

let now: t => float;

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

let scheduleAfter:
  (~delay: float, SchedulerContinuation.Work.t('state), 'state, t) =>
  Disposable.t;

let scheduleAfter1:
  (
    ~delay: float,
    SchedulerContinuation.Work.t1('state, 'ctx0),
    'state,
    'ctx0,
    t
  ) =>
  Disposable.t;

let scheduleAfter2:
  (
    ~delay: float,
    SchedulerContinuation.Work.t2('state, 'ctx0, 'ctx1),
    'state,
    'ctx0,
    'ctx1,
    t
  ) =>
  Disposable.t;

let scheduleAfter3:
  (
    ~delay: float,
    SchedulerContinuation.Work.t3('state, 'ctx0, 'ctx1, 'ctx2),
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
    SchedulerContinuation.Work.t4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3),
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

let scheduleAfter6:
  (
    ~delay: float,
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

let scheduleAfter7:
  (
    ~delay: float,
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

let scheduleBy: (~time: float, 'state => unit, 'state, t) => Disposable.t;

let scheduleBy1:
  (~time: float, ('ctx0, 'state) => unit, 'state, 'ctx0, t) => Disposable.t;

let scheduleBy2:
  (~time: float, ('ctx0, 'ctx1, 'state) => unit, 'state, 'ctx0, 'ctx1, t) =>
  Disposable.t;

let scheduleBy3:
  (
    ~time: float,
    ('ctx0, 'ctx1, 'ctx2, 'state) => unit,
    'state,
    'ctx0,
    'ctx1,
    'ctx2,
    t
  ) =>
  Disposable.t;

let scheduleBy4:
  (
    ~time: float,
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state) => unit,
    'state,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    t
  ) =>
  Disposable.t;

let scheduleBy5:
  (
    ~time: float,
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state) => unit,
    'state,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    t
  ) =>
  Disposable.t;

let scheduleBy6:
  (
    ~time: float,
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state) => unit,
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

let scheduleBy7:
  (
    ~time: float,
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'state) => unit,
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

let immediate: t;
type t = {
  executor: 'a 'state .unit => SchedulerExecutor.t('state, 'a),
  now: unit => float,
};

let now: t => float;

let schedule: (ScheduledWork.t('state), 'state, t) => Disposable.t;

let schedule1:
  (ScheduledWork.t1('ctx0, 'state), 'state, 'ctx0, t) => Disposable.t;

let schedule2:
  (ScheduledWork.t2('ctx0, 'ctx1, 'state), 'state, 'ctx0, 'ctx1, t) =>
  Disposable.t;

let schedule3:
  (
    ScheduledWork.t3('ctx0, 'ctx1, 'ctx2, 'state),
    'state,
    'ctx0,
    'ctx1,
    'ctx2,
    t
  ) =>
  Disposable.t;

let schedule4:
  (
    ScheduledWork.t4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state),
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
    ScheduledWork.t5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state),
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
    ScheduledWork.t6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state),
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
    ScheduledWork.t7('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'state),
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
  (~delay: float, ScheduledWork.t('state), 'state, t) => Disposable.t;

let scheduleAfter1:
  (~delay: float, ScheduledWork.t1('ctx0, 'state), 'state, 'ctx0, t) =>
  Disposable.t;

let scheduleAfter2:
  (
    ~delay: float,
    ScheduledWork.t2('ctx0, 'ctx1, 'state),
    'state,
    'ctx0,
    'ctx1,
    t
  ) =>
  Disposable.t;

let scheduleAfter3:
  (
    ~delay: float,
    ScheduledWork.t3('ctx0, 'ctx1, 'ctx2, 'state),
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
    ScheduledWork.t4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state),
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
    ScheduledWork.t5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state),
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
    ScheduledWork.t6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state),
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
    ScheduledWork.t7('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'state),
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
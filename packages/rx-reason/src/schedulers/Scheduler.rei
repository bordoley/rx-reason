type t = {
  now: unit => float,
  scheduleAfter: 'ctx .(~delay: float, 'ctx => unit, 'ctx) => Disposable.t,
  schedulePeriodic: 'ctx .(~delay: float, 'ctx => unit, 'ctx) => Disposable.t,
};

module RecursiveResult: {
  type t('state);

  let continue: 'state => t('state);
  let continueAfter: (~delay: float, 'state) => t('state);
  let done_: t('state);
};

let now: t => float;

let schedule: ('state => unit, 'state, t) => Disposable.t;

let schedule1: (('ctx0, 'state) => unit, 'state, 'ctx0, t) => Disposable.t;

let schedule2:
  (('ctx0, 'ctx1, 'state) => unit, 'state, 'ctx0, 'ctx1, t) => Disposable.t;

let schedule3:
  (('ctx0, 'ctx1, 'ctx2, 'state) => unit, 'state, 'ctx0, 'ctx1, 'ctx2, t) =>
  Disposable.t;

let schedule4:
  (
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state) => unit,
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

let schedule6:
  (
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

let schedule7:
  (
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

let scheduleAfter: (~delay: float, 'state => unit, 'state, t) => Disposable.t;

let scheduleAfter1:
  (~delay: float, ('ctx0, 'state) => unit, 'state, 'ctx0, t) => Disposable.t;

let scheduleAfter2:
  (~delay: float, ('ctx0, 'ctx1, 'state) => unit, 'state, 'ctx0, 'ctx1, t) =>
  Disposable.t;

let scheduleAfter3:
  (
    ~delay: float,
    ('ctx0, 'ctx1, 'ctx2, 'state) => unit,
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
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state) => unit,
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

let scheduleAfter6:
  (
    ~delay: float,
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

let scheduleAfter7:
  (
    ~delay: float,
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

let scheduleRecursive:
  (~delay: float, 'state => RecursiveResult.t('state), 'state, t) =>
  Disposable.t;

let scheduleRecursive1:
  (
    ~delay: float,
    ('ctx0, 'state) => RecursiveResult.t('state),
    'state,
    'ctx0,
    t
  ) =>
  Disposable.t;

let scheduleRecursive2:
  (
    ~delay: float,
    ('ctx0, 'ctx1, 'state) => RecursiveResult.t('state),
    'state,
    'ctx0,
    'ctx1,
    t
  ) =>
  Disposable.t;

let scheduleRecursive3:
  (
    ~delay: float,
    ('ctx0, 'ctx1, 'ctx2, 'state) => RecursiveResult.t('state),
    'state,
    'ctx0,
    'ctx1,
    'ctx2,
    t
  ) =>
  Disposable.t;

let scheduleRecursive4:
  (
    ~delay: float,
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state) => RecursiveResult.t('state),
    'state,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    t
  ) =>
  Disposable.t;

let scheduleRecursive5:
  (
    ~delay: float,
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state) => RecursiveResult.t('state),
    'state,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    t
  ) =>
  Disposable.t;

let scheduleRecursive6:
  (
    ~delay: float,
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state) =>
    RecursiveResult.t('state),
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

let scheduleRecursive7:
  (
    ~delay: float,
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'state) =>
    RecursiveResult.t('state),
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
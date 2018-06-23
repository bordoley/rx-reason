type t('state);

type schedulerContinuation('state) = t('state);

module Work: {
  type t('state) = (schedulerContinuation('state), 'state) => unit;
  type t1('state, 'ctx0) =
    ('ctx0, schedulerContinuation('state), 'state) => unit;
  type t2('state, 'ctx0, 'ctx1) =
    ('ctx0, 'ctx1, schedulerContinuation('state), 'state) => unit;
  type t3('state, 'ctx0, 'ctx1, 'ctx2) =
    ('ctx0, 'ctx1, 'ctx2, schedulerContinuation('state), 'state) => unit;
  type t4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3) =
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, schedulerContinuation('state), 'state) =>
    unit;
  type t5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4) =
    (
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      schedulerContinuation('state),
      'state
    ) =>
    unit;
  type t6('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5) =
    (
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      schedulerContinuation('state),
      'state
    ) =>
    unit;
  type t7('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6) =
    (
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      'ctx6,
      schedulerContinuation('state),
      'state
    ) =>
    unit;
};

module Executor: {
  type t('state) =
    (
      schedulerContinuation('state),
      'state,
      ('state, schedulerContinuation('state)) => unit
    ) =>
    unit;
};

include CompositeDisposable.S1 with type t('state) := t('state);

let continue: ('state, t('state)) => unit;

let create: (Executor.t('state), Work.t('state)) => t('state);

let create1:
  (Executor.t('state), Work.t1('state, 'ctx0), 'ctx0) => t('state);

let create2:
  (Executor.t('state), Work.t2('state, 'ctx0, 'ctx1), 'ctx0, 'ctx1) =>
  t('state);

let create3:
  (
    Executor.t('state),
    Work.t3('state, 'ctx0, 'ctx1, 'ctx2),
    'ctx0,
    'ctx1,
    'ctx2
  ) =>
  t('state);

let create4:
  (
    Executor.t('state),
    Work.t4('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3),
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3
  ) =>
  t('state);

let create5:
  (
    Executor.t('state),
    Work.t5('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4),
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4
  ) =>
  t('state);

let create6:
  (
    Executor.t('state),
    Work.t6('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5),
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5
  ) =>
  t('state);

let create7:
  (
    Executor.t('state),
    Work.t7('state, 'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6),
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    'ctx6
  ) =>
  t('state);
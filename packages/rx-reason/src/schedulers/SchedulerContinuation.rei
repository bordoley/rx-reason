type t('state);

type schedulerContinuation('state) = t('state);

module Executor: {
  type t('state, 'a) =
    (~delay: float, 'a, 'state, ('state, 'a) => unit) => Disposable.t;
};

include SerialDisposable.S1 with type t('state) := t('state);

let continue: ('state, t('state)) => unit;

let continueAfter: (~delay: float, 'state, t('state)) => unit;

let create: (Executor.t('state, t('state)), ScheduledWork.t('state)) => t('state);

let create1:
  (Executor.t('state, t('state)), ScheduledWork.t1('ctx0, 'state), 'ctx0) => t('state);

let create2:
  (
    Executor.t('state, t('state)),
    ScheduledWork.t2('ctx0, 'ctx1, 'state),
    'ctx0,
    'ctx1
  ) =>
  t('state);

let create3:
  (
    Executor.t('state, t('state)),
    ScheduledWork.t3('ctx0, 'ctx1, 'ctx2, 'state),
    'ctx0,
    'ctx1,
    'ctx2
  ) =>
  t('state);

let create4:
  (
    Executor.t('state, t('state)),
    ScheduledWork.t4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state),
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3
  ) =>
  t('state);

let create5:
  (
    Executor.t('state, t('state)),
    ScheduledWork.t5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state),
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4
  ) =>
  t('state);

let create6:
  (
    Executor.t('state, t('state)),
    ScheduledWork.t6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state),
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
    Executor.t('state, t('state)),
    ScheduledWork.t7('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'state),
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    'ctx6
  ) =>
  t('state);

let disposed: t('state);
/**
 * A provider of push-based notifications.
 */;
type t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  include RxObservableLike.S1 with type t('a) := t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);
};

include RxObservableLike.S1 with type t('a) := t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create: (RxSubscriber.t('a) => unit) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create1: (('ctx0, RxSubscriber.t('a)) => unit, 'ctx0) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create2:
  (('ctx0, 'ctx1, RxSubscriber.t('a)) => unit, 'ctx0, 'ctx1) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create3:
  (('ctx0, 'ctx1, 'ctx2, RxSubscriber.t('a)) => unit, 'ctx0, 'ctx1, 'ctx2) =>
  t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create4:
  (
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, RxSubscriber.t('a)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3
  ) =>
  t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create5:
  (
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, RxSubscriber.t('a)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4
  ) =>
  t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create6:
  (
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, RxSubscriber.t('a)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5
  ) =>
  t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create7:
  (
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, RxSubscriber.t('a)) =>
    unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    'ctx6
  ) =>
  t('a);

/**
 * Returns an Observable that applies the Operator function to the
 * source Observable's notifications.
 */
let lift: (RxOperator.t('a, 'b), t('a)) => t('b);

/**
 * Returns an Observable the emits no values and never completes.
 */
let never: t('a);
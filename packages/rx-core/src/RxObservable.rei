/**
 * A provider of push-based notifications.
 */;
type t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);
};

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
 * Returns an Observable that applies the Operator function to the
 * source Observable's notifications.
 */
let lift: (RxOperator.t('a, 'b), t('a)) => t('b);

/**
 * Returns an Observable the emits no values and never completes.
 */
let never: t('a);

/**
 * Returns an Observable which invokes side-effect functions on each
 * observed event and on completion.
 */
let observe:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) => t('a);

/**
 * Returns an Observable which invokes side-effect functions on each
 * observed event and on completion.
 */
let observe1:
  (
    ~onNext: ('ctx0, 'a) => unit,
    ~onComplete: ('ctx0, option(exn)) => unit,
    'ctx0,
    t('a)
  ) =>
  t('a);

/**
 * Returns an Observable which invokes side-effect functions on each
 * observed event and on completion.
 */
let observe2:
  (
    ~onNext: ('ctx0, 'ctx1, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit,
    'ctx0,
    'ctx1,
    t('a)
  ) =>
  t('a);

/**
 * Returns an Observable which invokes side-effect functions on each
 * observed event and on completion.
 */
let observe3:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    t('a)
  ) =>
  t('a);

/**
 * Returns an Observable which invokes side-effect functions on each
 * observed event and on completion.
 */
let observe4:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    t('a)
  ) =>
  t('a);

/**
 * Returns an Observable which invokes side-effect functions on each
 * observed event and on completion.
 */
let observe5:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    t('a)
  ) =>
  t('a);

let subscribe: t('a) => RxDisposable.t;
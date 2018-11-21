/**
 * A provider of push-based notifications.
 */;
type t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  include ObservableLike.S1 with type t('a) := t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);
};

include ObservableLike.S1 with type t('a) := t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create: (Subscriber.t('a) => unit) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create1: (('ctx0, Subscriber.t('a)) => unit, 'ctx0) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create2:
  (('ctx0, 'ctx1, Subscriber.t('a)) => unit, 'ctx0, 'ctx1) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create3:
  (('ctx0, 'ctx1, 'ctx2, Subscriber.t('a)) => unit, 'ctx0, 'ctx1, 'ctx2) =>
  t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create4:
  (
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, Subscriber.t('a)) => unit,
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
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, Subscriber.t('a)) => unit,
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
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, Subscriber.t('a)) => unit,
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
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, Subscriber.t('a)) => unit,
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
let lift: (Operator.t('a, 'b), t('a)) => t('b);

/**
 * Returns an Observable the emits no values and never completes.
 */
let never: t('a);

let pipe2: (Operator.t('a, 'b), Operator.t('b, 'c), t('a)) => t('c);

let pipe3:
  (Operator.t('a, 'b), Operator.t('b, 'c), Operator.t('c, 'd), t('a)) =>
  t('d);

let pipe4:
  (
    Operator.t('a, 'b),
    Operator.t('b, 'c),
    Operator.t('c, 'd),
    Operator.t('d, 'e),
    t('a)
  ) =>
  t('e);

let pipe5:
  (
    Operator.t('a, 'b),
    Operator.t('b, 'c),
    Operator.t('c, 'd),
    Operator.t('d, 'e),
    Operator.t('e, 'f),
    t('a)
  ) =>
  t('f);

let pipe6:
  (
    Operator.t('a, 'b),
    Operator.t('b, 'c),
    Operator.t('c, 'd),
    Operator.t('d, 'e),
    Operator.t('e, 'f),
    Operator.t('f, 'g),
    t('a)
  ) =>
  t('g);

let pipe7:
  (
    Operator.t('a, 'b),
    Operator.t('b, 'c),
    Operator.t('c, 'd),
    Operator.t('d, 'e),
    Operator.t('e, 'f),
    Operator.t('f, 'g),
    Operator.t('g, 'h),
    t('a)
  ) =>
  t('h);

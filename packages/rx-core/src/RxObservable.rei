/**
 * A provider of push-based notifications.
 */;

 /** The RxObservable type. */
type t('a);

type observable('a) = t('a);

/** RxObservable module type signature for types with a parametric type arity of 0. */
module type S = {
  type a;
  type t;

  /** Cast to RxObservable.t. */
  let asObservable: t => observable(a);
};

/** RxObservable module type signature for types with a parametric type arity of 0. */
module type S1 = {
  type t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);
};

include RxConnectableLike.S1 with type t('a) := t('a);

/**
 * Returns an RxObservable using the provided subscribe function.
 */
let create: (RxSubscriber.t('a) => unit) => t('a);

/**
 * Returns an RxObservable using the provide subscribe function and 
 * context variable.
 */
let create1: (('ctx0, RxSubscriber.t('a)) => unit, 'ctx0) => t('a);

/**
 * Returns an RxObservable using the provide subscribe function and 
 * context variables.
 */
let create2:
  (('ctx0, 'ctx1, RxSubscriber.t('a)) => unit, 'ctx0, 'ctx1) => t('a);

/**
 * Returns an RxObservable using the provide subscribe function and 
 * context variables.
 */
let create3:
  (('ctx0, 'ctx1, 'ctx2, RxSubscriber.t('a)) => unit, 'ctx0, 'ctx1, 'ctx2) =>
  t('a);

/**
 * Returns an RxObservable using the provide subscribe function and 
 * context variables.
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
 * Returns an RxObservable using the provide subscribe function and 
 * context variables.
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
 * Returns an RxObservable that applies the RxOperator function to the
 * source RxObservable's notifications.
 */
let lift: (RxOperator.t('a, 'b), t('a)) => t('b);

/**
 * Returns an RxObservable that emits no values and never completes.
 */
let never: t('a);
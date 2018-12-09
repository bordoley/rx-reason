/**
 * Materialized representation of reactive events. 
 * 
 * For performance reasons reactive events are not materialized by default, but they can 
 * be useful for implementation of certain operators.
 */;

/** The RxNotification type. */
type t('a);

/** Returns a completed notification with the provided optional exception. */
let complete: option(exn) => t('a);

/** Returns a next notification with the provided value. */
let next: 'a => t('a);

/** 
 * Applies the provided mapping functions to the underlying 
 * notification returning the resulting value. 
 */
let map: (~onNext: 'a => 'b, ~onComplete: option(exn) => 'b, t('a)) => 'b;

/** 
 * Applies the mapping functions with the provided context variable 
 * to the underlying notification returning the resulting value. 
 */
let map1:
  (
    ~onNext: ('ctx0, 'a) => 'b,
    ~onComplete: ('ctx0, option(exn)) => 'b,
    'ctx0,
    t('a)
  ) =>
  'b;

/** 
 * Applies the mapping functions with the provided context variables 
 * to the underlying notification returning the resulting value. 
 */
let map2:
  (
    ~onNext: ('ctx0, 'ctx1, 'a) => 'b,
    ~onComplete: ('ctx0, 'ctx1, option(exn)) => 'b,
    'ctx0,
    'ctx1,
    t('a)
  ) =>
  'b;
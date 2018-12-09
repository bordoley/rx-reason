/**
 * Module type signatures for types that support observing push based notifications.
 */;

/** RxObserverLike module type signature for types with a parametric type arity of 0. */
module type S = {
  type a;
  type t;

  /** Notify the Observer that no more notifications will be sent, optionally with an exception. */
  let complete: (~exn: exn=?, t) => unit;

  /**
   * Notify the Observer that no more notifications will be sent, optionally with an exception.
   * Returns true if the Observer has not previously been completed, otherwise false.
   */
  let completeWithResult: (~exn: exn=?, t) => bool;

  /** Notify the Observer of the next element to observe. */
  let next: (a, t) => unit;

  /** Notify the Observer of the next notification to observe. */
  let notify: (RxNotification.t(a), t) => unit;
};

/** RxObserverLike module type signature for types with a parametric type arity of 1. */
module type S1 = {
  type t('a);

  /** Notify the Observer that no more notifications will be sent, optionally with an exception. */
  let complete: (~exn: exn=?, t('a)) => unit;

  /**
   * Notify the Observer that no more notifications will be sent, optionally with an exception.
   * Returns true if the Observer has not previously been completed, otherwise false.
   */
  let completeWithResult: (~exn: exn=?, t('a)) => bool;

  /** Notify the Observer of the next element to observe. */
  let next: ('a, t('a)) => unit;

  /** Notify the Observer of the next notification to observe. */
  let notify: (RxNotification.t('a), t('a)) => unit;
};
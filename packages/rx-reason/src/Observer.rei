/**
 * Represents the underlying subscription to an Observable and it's callbacks.
 * Observers are a specialized type, primarily used in the implementation of
 * Operators. Normal use cases, such as subscribing to an Observable, should use
 * callback functions instead.
 */;
type t('a);

type observer('a) = t('a);

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  /** Cast to Observer.t. */
  let asObserver: t('a) => observer('a);

  /** Notify the observer that no more notifications will be sent, optionally with an exception. */
  let complete: (~exn: exn=?, t('a)) => unit;

  /** 
   * Notify the Observer that no more notifications will be sent, optionally with an exception. 
   * Returns true if the Observer has not previously been completed, otherwise false.
   */
  let completeWithResult: (~exn: exn=?, t('a)) => bool;

  /** Returns true if the Observer has been completed or disposed. */
  let isStopped: t('a) => bool;
  
  /* Notify the observer of the next element to observe. */
  let next: ('a, t('a)) => unit;
  
  /* Notify the observer of the next notification to observe. */
  let notify: (Notification.t('a), t('a)) => unit;
};

include S1 with type t('a) := t('a);

/** Construct a new Observer with the provided callbacks. */
let create:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    ~onDispose: unit => unit
  ) =>
  t('a);

/** 
 * Construct a new Observer with the provided callbacks which automatically disposes 
 * itself when completed. 
 * */
let createAutoDisposing:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    ~onDispose: unit => unit
  ) =>
  t('a);

/**
 * A disposed Observer instance.
 */
let disposed: t('a);

/** Returns an onComplete function which forwards the notifcation to the provided Observer. */
let forwardOnComplete: (t('a), option(exn)) => unit;

/** Returns an onNext function which forwards the notifcation to the provided Observer. */
let forwardOnNext: (t('a), 'a) => unit;

/** Returns an onDispose function which forwards the notifcation to the provided Observer. */
let forwardOnDispose: (t('a), unit) => unit;
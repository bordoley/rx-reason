/**
 * A type that both an Observable sequence as well as an Observer.
 */;
type t('a);

include Disposable.S1 with type t('a) := t('a);
include Observable.S1 with type t('a) := t('a);

/** Notify the observer that no more notifications will be sent, optionally with an exception. */
let complete: (~exn: exn=?, t('a)) => unit;

/**
   * Notify the Observer that no more notifications will be sent, optionally with an exception.
   * Returns true if the Observer has not previously been completed, otherwise false.
   */
let completeWithResult: (~exn: exn=?, t('a)) => bool;

/** Constructs a new Subject instance. */
let create: unit => t('a);

/**
 * Constructs a new Subject instance with the specified buffer size
 * which will replay the most recent buffered notifications when
 * subscribed to.
 * */
let createWithReplayBuffer: int => t('a);

/**
 * A disposed Subject instance.
 */
let disposed: t('a);

/* Notify the observer of the next element to observe. */
let next: ('a, t('a)) => unit;

/* Notify the observer of the next notification to observe. */
let notify: (Notification.t('a), t('a)) => unit;
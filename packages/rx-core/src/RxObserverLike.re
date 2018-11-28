module type S1 = {
  type t('a);

  /** Notify the Observer that no more notifications will be sent, optionally with an exception. */
  let complete: (~exn: exn=?, t('a)) => unit;

  /* Notify the Observer of the next element to observe. */
  let next: ('a, t('a)) => unit;

  /* Notify the Observer of the next notification to observe. */
  let notify: (RxNotification.t('a), t('a)) => unit;
};
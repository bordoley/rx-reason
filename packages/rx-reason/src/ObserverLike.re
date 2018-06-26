module type S1 = {
  type t('a);

  /** Notify the Observer that no more notifications will be sent, optionally with an exception. */
  let complete: (~exn: exn=?, t('a)) => unit;

  /**
   * Notify the Observer that no more notifications will be sent, optionally with an exception.
   * Returns true if the Observer has not previously been completed, otherwise false.
   */
  let completeWithResult: (~exn: exn=?, t('a)) => bool;

  /* Notify the Observer of the next element to observe. */
  let next: ('a, t('a)) => unit;

  /* Notify the Observer of the next notification to observe. */
  let notify: (Notification.t('a), t('a)) => unit;

  let delegateOnComplete: (t('a), option(exn)) => unit;
  let delegateOnComplete1: ('ctx0, t('a), option(exn)) => unit;
  let delegateOnComplete2: ('ctx0, 'ctx1, t('a), option(exn)) => unit;
  let delegateOnComplete3: ('ctx0, 'ctx1, 'ctx2, t('a), option(exn)) => unit;
  let delegateOnComplete4:
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('a), option(exn)) => unit;
  let delegateOnComplete5:
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('a), option(exn)) => unit;
  let delegateOnComplete6:
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('a), option(exn)) => unit;
  let delegateOnComplete7:
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, t('a), option(exn)) =>
    unit;

  let delegateOnNext: (t('a), 'a) => unit;
  let delegateOnNext1: ('ctx0, t('a), 'a) => unit;
  let delegateOnNext2: ('ctx0, 'ctx1, t('a), 'a) => unit;
  let delegateOnNext3: ('ctx0, 'ctx1, 'ctx2, t('a), 'a) => unit;
  let delegateOnNext4: ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('a), 'a) => unit;
  let delegateOnNext5: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('a), 'a) => unit;
  let delegateOnNext6:
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('a), 'a) => unit;
  let delegateOnNext7:
    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, t('a), 'a) => unit;
};
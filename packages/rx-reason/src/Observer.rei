type t('a);

type observer('a) = t('a);

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let complete: (~exn: exn=?, t('a)) => unit;
  let completeWithResult: (~exn: exn=?, t('a)) => bool;
  let isStopped: t('a) => bool;
  let next: ('a, t('a)) => unit;
  let notify: (Notification.t('a), t('a)) => unit;
  let toObserver: t('a) => observer('a);
};

include S1 with type t('a) := t('a);

let create:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    ~onDispose: unit => unit
  ) =>
  t('a);

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

let forwardOnComplete: (t('a), option(exn)) => unit;

let forwardOnNext: (t('a), 'a) => unit;

let forwardOnDispose: (t('a), unit) => unit;
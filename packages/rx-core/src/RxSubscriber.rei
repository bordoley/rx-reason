/**
 * Represents the underlying subscription to an Observable and it's callbacks.
 * Subscribers are a specialized type, primarily used in the implementation of
 * Operators. Normal use cases, such as subscribing to an Observable, should use
 * callback functions instead.
 */;
type t('a);

include RxCompositeDisposable.S1 with type t('a) := t('a);
include RxObserverLike.S1 with type t('a) := t('a);

/**
 * Notify the Observer that no more notifications will be sent, optionally with an exception.
 * Returns true if the Observer has not previously been completed, otherwise false.
 */
let completeWithResult: (~exn: exn=?, t('a)) => bool;

/**
 * Construct a new Subscriber with the provided callbacks which automatically disposes
 * itself when completed.
 * */
let create: unit => t('a);

/** Construct a new Subscriber with the provided callbacks that decorates its disposal to another Subscriber. */
let decorate:
  (
    ~onNext: (t('b), 'a) => unit,
    ~onComplete: (t('b), option(exn)) => unit,
    t('b)
  ) =>
  t('a);

let decorate1:
  (
    ~onNext: ('ctx0, t('b), 'a) => unit,
    ~onComplete: ('ctx0, t('b), option(exn)) => unit,
    'ctx0,
    t('b)
  ) =>
  t('a);

let decorate2:
  (
    ~onNext: ('ctx0, 'ctx1, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    t('b)
  ) =>
  t('a);

let decorate3:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    t('b)
  ) =>
  t('a);

let decorate4:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    t('b)
  ) =>
  t('a);

let decorate5:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), option(exn)) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    t('b)
  ) =>
  t('a);

let forwardOnComplete: (t('a), option(exn)) => unit;
let forwardOnComplete1: ('ctx0, t('a), option(exn)) => unit;
let forwardOnComplete2: ('ctx0, 'ctx1, t('a), option(exn)) => unit;
let forwardOnComplete3: ('ctx0, 'ctx1, 'ctx2, t('a), option(exn)) => unit;
let forwardOnComplete4:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('a), option(exn)) => unit;
let forwardOnComplete5:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('a), option(exn)) => unit;

let forwardOnNext: (t('a), 'a) => unit;
let forwardOnNext1: ('ctx0, t('a), 'a) => unit;
let forwardOnNext2: ('ctx0, 'ctx1, t('a), 'a) => unit;
let forwardOnNext3: ('ctx0, 'ctx1, 'ctx2, t('a), 'a) => unit;
let forwardOnNext4: ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('a), 'a) => unit;
let forwardOnNext5: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('a), 'a) => unit;

/**
 * A disposed Subscriber instance.
 */
let disposed: t('a);

/** Returns true if the Observer has been completed or disposed. */
let isStopped: t('a) => bool;
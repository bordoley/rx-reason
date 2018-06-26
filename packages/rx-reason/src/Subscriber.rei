/**
 * Represents the underlying subscription to an Observable and it's callbacks.
 * Subscribers are a specialized type, primarily used in the implementation of
 * Operators. Normal use cases, such as subscribing to an Observable, should use
 * callback functions instead.
 */;
type t('a);

include CompositeDisposable.S1 with type t('a) := t('a);
include ObserverLike.S1 with type t('a) := t('a);

/**
 * Construct a new Subscriber with the provided callbacks which automatically disposes
 * itself when completed.
 * */
let createAutoDisposing:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) => t('a);

let createAutoDisposing1:
  (
    ~onNext: ('ctx0, 'a) => unit,
    ~onComplete: ('ctx0, option(exn)) => unit,
    'ctx0
  ) =>
  t('a);

let createAutoDisposing2:
  (
    ~onNext: ('ctx0, 'ctx1, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit,
    'ctx0,
    'ctx1
  ) =>
  t('a);

let createAutoDisposing3:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2
  ) =>
  t('a);

let createAutoDisposing4:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3
  ) =>
  t('a);

let createAutoDisposing5:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4
  ) =>
  t('a);

let createAutoDisposing6:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, option(exn)) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5
  ) =>
  t('a);

let createAutoDisposing7:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'a) => unit,
    ~onComplete: (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   'ctx6,
                   option(exn)
                 ) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    'ctx6
  ) =>
  t('a);

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

let decorate6:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('b), 'a) => unit,
    ~onComplete: (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   t('b),
                   option(exn)
                 ) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    t('b)
  ) =>
  t('a);

let decorate7:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, t('b), 'a) =>
             unit,
    ~onComplete: (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   'ctx6,
                   t('b),
                   option(exn)
                 ) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    'ctx6,
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
let forwardOnComplete6:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('a), option(exn)) => unit;
let forwardOnComplete7:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, t('a), option(exn)) =>
  unit;

let forwardOnNext: (t('a), 'a) => unit;
let forwardOnNext1: ('ctx0, t('a), 'a) => unit;
let forwardOnNext2: ('ctx0, 'ctx1, t('a), 'a) => unit;
let forwardOnNext3: ('ctx0, 'ctx1, 'ctx2, t('a), 'a) => unit;
let forwardOnNext4: ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('a), 'a) => unit;
let forwardOnNext5: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('a), 'a) => unit;
let forwardOnNext6:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('a), 'a) => unit;
let forwardOnNext7:
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, t('a), 'a) => unit;

/**
 * A disposed Subscriber instance.
 */
let disposed: t('a);

/** Returns true if the Observer has been completed or disposed. */
let isStopped: t('a) => bool;
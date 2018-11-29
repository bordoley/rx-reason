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

/**
 * A disposed Subscriber instance.
 */
let disposed: t('a);
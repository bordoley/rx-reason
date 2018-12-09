/**
 * Represents the underlying subscription to an RxObservable and it's callbacks.
 * RxSubscribers are a specialized type used in the implementation of
 * RxOperators.
 */;

/** The RxSubscriber type */
type t('a);

include RxCompositeDisposable.S1 with type t('a) := t('a);
include RxObserverLike.S1 with type t('a) := t('a);

/** Returns an RxSubscriber which automatically disposes itself when completed. */
let create: unit => t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided callbacks.
 */
let decorate:
  (
    ~onNext: (. t('b), 'a) => unit,
    ~onComplete: (. t('b), option(exn)) => unit,
    t('b)
  ) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onComplete function, while forwarding onNext calls
 * to the decorated subscriber.
 */
let decorateOnComplete: ((. t('a), option(exn)) => unit, t('a)) => t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onNext function, while forwarding onComplete calls
 * to the decorated subscriber.
 */
let decorateOnNext: ((. t('b), 'a) => unit, t('b)) => t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided callbacks.
 */
let decorate1:
  (
    ~onNext: (.'ctx0, t('b), 'a) => unit,
    ~onComplete: (.'ctx0, t('b), option(exn)) => unit,
    'ctx0,
    t('b)
  ) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onComplete function, while forwarding onNext calls
 * to the decorated subscriber.
 */
let decorateOnComplete1:
  ((.'ctx0, t('a), option(exn)) => unit, 'ctx0, t('a)) => t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onNext function, while forwarding onComplete calls
 * to the decorated subscriber.
 */
let decorateOnNext1: ((.'ctx0, t('b), 'a) => unit, 'ctx0, t('b)) => t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided callbacks.
 */
let decorate2:
  (
    ~onNext: (.'ctx0, 'ctx1, t('b), 'a) => unit,
    ~onComplete: (.'ctx0, 'ctx1, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    t('b)
  ) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onComplete function, while forwarding onNext calls
 * to the decorated subscriber.
 */
let decorateOnComplete2:
  ((.'ctx0, 'ctx1, t('a), option(exn)) => unit, 'ctx0, 'ctx1, t('a)) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onNext function, while forwarding onComplete calls
 * to the decorated subscriber.
 */
let decorateOnNext2:
  ((.'ctx0, 'ctx1, t('b), 'a) => unit, 'ctx0, 'ctx1, t('b)) => t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided callbacks.
 */
let decorate3:
  (
    ~onNext: (.'ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
    ~onComplete: (.'ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    t('b)
  ) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onComplete function, while forwarding onNext calls
 * to the decorated subscriber.
 */
let decorateOnComplete3:
  (
    (.'ctx0, 'ctx1, 'ctx2, t('a), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    t('a)
  ) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onNext function, while forwarding onComplete calls
 * to the decorated subscriber.
 */
let decorateOnNext3:
  ((.'ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit, 'ctx0, 'ctx1, 'ctx2, t('b)) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided callbacks.
 */
let decorate4:
  (
    ~onNext: (.'ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), 'a) => unit,
    ~onComplete: (.'ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    t('b)
  ) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onComplete function, while forwarding onNext calls
 * to the decorated subscriber.
 */
let decorateOnComplete4:
  (
    (.'ctx0, 'ctx1, 'ctx2, 'ctx3, t('a), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    t('a)
  ) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onNext function, while forwarding onComplete calls
 * to the decorated subscriber.
 */
let decorateOnNext4:
  (
    (.'ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), 'a) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    t('b)
  ) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided callbacks.
 */
let decorate5:
  (
    ~onNext: (.'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), 'a) => unit,
    ~onComplete: (.'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), option(exn)) =>
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
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onComplete function, while forwarding onNext calls
 * to the decorated subscriber.
 */
let decorateOnComplete5:
  (
    (.'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('a), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    t('a)
  ) =>
  t('a);

/**
 * Returns an RxSubscriber which decorates the provided subscriber
 * with the provided onNext function, while forwarding onComplete calls
 * to the decorated subscriber.
 */
let decorateOnNext5:
  (
    (.'ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), 'a) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    t('b)
  ) =>
  t('a);

/**
 * A disposed RxSubscriber instance.
 */
let disposed: t('a);
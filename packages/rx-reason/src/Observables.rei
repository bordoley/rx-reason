/**
 * Returns an Observable which concatenates elements of each provided Observable
 * into a single sequence of elements without interleaving them.
 */
let concat:
  (~scheduler: Scheduler.t=?, list(Observable.t('a))) => Observable.t('a);

/**
 * Returns an Observable that calls an Observable factory to
 * create a new Observable for each subscription.
 */
let defer: (unit => Observable.t('a)) => Observable.t('a);

/**
 * Returns an Observable that emits no items and immediately completes.
 */
let empty: (~scheduler: Scheduler.t=?, unit) => Observable.t('a);

/**
 * Returns an Observable that merges items emitted by the source Observables,
 * interleaving the items emitted by each Observable.
 */
let merge: list(Observable.t('a)) => Observable.t('a);

/**
 * Returns an Observable which emits the specified notifications at the
 * specified absolutes times using the supplied scheduler.
 */
let ofAbsoluteTimeNotifications:
  (~scheduler: Scheduler.t, list((float, Notification.t('a)))) =>
  Observable.t('a);

/**
 * Returns an Observable that emits the given items and then completes.
 */
let ofList: (~scheduler: Scheduler.t=?, list('a)) => Observable.t('a);

/**
 * Returns an Observable which emits the specified notifications
 * using the supplied scheduler.
 */
let ofNotifications:
  (~scheduler: Scheduler.t=?, list(Notification.t('a))) => Observable.t('a);

/**
 * Returns an Observable which emits the specified notifications at the
 * specified relative times using the supplied scheduler.
 */
let ofRelativeTimeNotifications:
  (~scheduler: Scheduler.t, list((float, Notification.t('a)))) =>
  Observable.t('a);

/**
  * Returns an Observable that emits the given item and then completes.
  */
let ofValue: (~scheduler: Scheduler.t=?, 'a) => Observable.t('a);

let publish:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    Observable.t('a),
    unit
  ) =>
  Disposable.t;

let publish1:
  (
    ~onNext: ('ctx0, 'a) => unit,
    ~onComplete: ('ctx0, option(exn)) => unit,
    'ctx0,
    Observable.t('a),
    unit
  ) =>
  Disposable.t;

let publish2:
  (
    ~onNext: ('ctx0, 'ctx1, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit,
    'ctx0,
    'ctx1,
    Observable.t('a),
    unit
  ) =>
  Disposable.t;

let publish3:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    Observable.t('a),
    unit
  ) =>
  Disposable.t;

let publish4:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    Observable.t('a),
    unit
  ) =>
  Disposable.t;

let publish5:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    Observable.t('a),
    unit
  ) =>
  Disposable.t;

let publish6:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) => unit,
    ~onComplete: (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   option(exn)
                 ) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    Observable.t('a),
    unit
  ) =>
  Disposable.t;

let publish7:
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
    'ctx6,
    Observable.t('a),
    unit
  ) =>
  Disposable.t;

/**
 * Returns an Observable that completes with the specified exception
 * when subscribed to.
 */
let raise: (~scheduler: Scheduler.t=?, exn) => Observable.t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * resubscribing to it if it completes and
 * the predicate returns true for that specific.
 */
let repeat:
  (~predicate: unit => bool=?, Observable.t('a)) => Observable.t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * resubscribing to it if it completes with an exception and
 * the predicate returns true for that specific exception.
 */
let retry: (~predicate: exn => bool=?, Observable.t('a)) => Observable.t('a);

/**
 * Returns an Observable that emits the specified items before it
 * begins to emit items emitted by the source Observable.
 */
let startWithList:
  (~scheduler: Scheduler.t=?, list('a), Observable.t('a)) =>
  Observable.t('a);

/**
 * Returns an Observable that emits a specified item before it
 * begins to emit items emitted by the source Observable.
 */
let startWithValue:
  (~scheduler: Scheduler.t=?, 'a, Observable.t('a)) => Observable.t('a);

/**
 * Returns an Observable which subscribes to the provided source Observable
 * on the specified scheduler.
 */
let subscribeOn:
  (~delay: float=?, Scheduler.t, Observable.t('a)) => Observable.t('a);

/**
 * Returns an Observable which collects all observed items in a list
 * and emits a single list of observed values in the observed
 * order when completed.
 *
 * Note: This Operator is intended to support testing. Using this
 * operator on unbounded Observable sources may lead to memory exhaustion.
 */
let toList: Observable.t('a) => Observable.t(list('a));
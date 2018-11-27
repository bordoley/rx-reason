/**
 * Returns an Observable which concatenates elements of each provided Observable
 * into a single sequence of elements without interleaving them.
 */
let concat:
  (~scheduler: RxScheduler.t=?, list(RxObservable.t('a))) =>
  RxObservable.t('a);

/**
 * Returns an Observable that calls an Observable factory to
 * create a new Observable for each subscription.
 */
let defer: (unit => RxObservable.t('a)) => RxObservable.t('a);

/**
 * Returns an Observable that emits no items and immediately completes.
 */
let empty: (~scheduler: RxScheduler.t=?, unit) => RxObservable.t('a);

/**
 * Returns an Observable that merges items emitted by the source Observables,
 * interleaving the items emitted by each Observable.
 */
let merge: list(RxObservable.t('a)) => RxObservable.t('a);

/**
 * Returns an Observable which emits the specified notifications at the
 * specified absolutes times using the supplied scheduler.
 */
let ofAbsoluteTimeNotifications:
  (~scheduler: RxScheduler.t, list((float, RxNotification.t('a)))) =>
  RxObservable.t('a);

/**
 * Returns an Observable that emits the given items and then completes.
 */
let ofList: (~scheduler: RxScheduler.t=?, list('a)) => RxObservable.t('a);

/**
 * Returns an Observable which emits the specified notifications
 * using the supplied scheduler.
 */
let ofNotifications:
  (~scheduler: RxScheduler.t=?, list(RxNotification.t('a))) =>
  RxObservable.t('a);

/**
 * Returns an Observable which emits the specified notifications at the
 * specified relative times using the supplied scheduler.
 */
let ofRelativeTimeNotifications:
  (~scheduler: RxScheduler.t, list((float, RxNotification.t('a)))) =>
  RxObservable.t('a);

/**
  * Returns an Observable that emits the given item and then completes.
  */
let ofValue: (~scheduler: RxScheduler.t=?, 'a) => RxObservable.t('a);

let pipe2:
  (RxOperator.t('a, 'b), RxOperator.t('b, 'c), RxObservable.t('a)) =>
  RxObservable.t('c);

let pipe3:
  (
    RxOperator.t('a, 'b),
    RxOperator.t('b, 'c),
    RxOperator.t('c, 'd),
    RxObservable.t('a)
  ) =>
  RxObservable.t('d);

let pipe4:
  (
    RxOperator.t('a, 'b),
    RxOperator.t('b, 'c),
    RxOperator.t('c, 'd),
    RxOperator.t('d, 'e),
    RxObservable.t('a)
  ) =>
  RxObservable.t('e);

let pipe5:
  (
    RxOperator.t('a, 'b),
    RxOperator.t('b, 'c),
    RxOperator.t('c, 'd),
    RxOperator.t('d, 'e),
    RxOperator.t('e, 'f),
    RxObservable.t('a)
  ) =>
  RxObservable.t('f);

let pipe6:
  (
    RxOperator.t('a, 'b),
    RxOperator.t('b, 'c),
    RxOperator.t('c, 'd),
    RxOperator.t('d, 'e),
    RxOperator.t('e, 'f),
    RxOperator.t('f, 'g),
    RxObservable.t('a)
  ) =>
  RxObservable.t('g);

let pipe7:
  (
    RxOperator.t('a, 'b),
    RxOperator.t('b, 'c),
    RxOperator.t('c, 'd),
    RxOperator.t('d, 'e),
    RxOperator.t('e, 'f),
    RxOperator.t('f, 'g),
    RxOperator.t('g, 'h),
    RxObservable.t('a)
  ) =>
  RxObservable.t('h);

let publish:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    RxObservable.t('a),
    unit
  ) =>
  RxDisposable.t;

let publish1:
  (
    ~onNext: ('ctx0, 'a) => unit,
    ~onComplete: ('ctx0, option(exn)) => unit,
    'ctx0,
    RxObservable.t('a),
    unit
  ) =>
  RxDisposable.t;

let publish2:
  (
    ~onNext: ('ctx0, 'ctx1, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit,
    'ctx0,
    'ctx1,
    RxObservable.t('a),
    unit
  ) =>
  RxDisposable.t;

let publish3:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    RxObservable.t('a),
    unit
  ) =>
  RxDisposable.t;

let publish4:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    RxObservable.t('a),
    unit
  ) =>
  RxDisposable.t;

let publish5:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    RxObservable.t('a),
    unit
  ) =>
  RxDisposable.t;

let publish6:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, option(exn)) =>
                 unit,
    'ctx0,
    'ctx1,
    'ctx2,
    'ctx3,
    'ctx4,
    'ctx5,
    RxObservable.t('a),
    unit
  ) =>
  RxDisposable.t;

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
    RxObservable.t('a),
    unit
  ) =>
  RxDisposable.t;

/**
 * Returns an Observable that completes with the specified exception
 * when subscribed to.
 */
let raise: (~scheduler: RxScheduler.t=?, exn) => RxObservable.t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * resubscribing to it if it completes and
 * the predicate returns true for that specific.
 */
let repeat:
  (~predicate: unit => bool=?, RxObservable.t('a)) => RxObservable.t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * resubscribing to it if it completes with an exception and
 * the predicate returns true for that specific exception.
 */
let retry:
  (~predicate: exn => bool=?, RxObservable.t('a)) => RxObservable.t('a);

/**
 * Returns a new MulticastObservable that shares a single subscription to the provided source Observable.
 */
let share: RxObservable.t('a) => RxObservable.t('a);

/**
 * Returns a new MulticastObservable that shares a single subscription to the provided source Observable.
 * In addition, it replays the last n events each time it is subscribed to.
 */
let shareWithReplayBuffer: (int, RxObservable.t('a)) => RxObservable.t('a);

/**
 * Returns an Observable that emits the specified items before it
 * begins to emit items emitted by the source Observable.
 */
let startWithList:
  (~scheduler: RxScheduler.t=?, list('a), RxObservable.t('a)) =>
  RxObservable.t('a);

/**
 * Returns an Observable that emits a specified item before it
 * begins to emit items emitted by the source Observable.
 */
let startWithValue:
  (~scheduler: RxScheduler.t=?, 'a, RxObservable.t('a)) => RxObservable.t('a);

/**
 * Returns an Observable which subscribes to the provided source Observable
 * on the specified scheduler.
 */
let subscribeOn:
  (~delay: float=?, RxScheduler.t, RxObservable.t('a)) => RxObservable.t('a);

/**
 * Returns an Observable which collects all observed items in a list
 * and emits a single list of observed values in the observed
 * order when completed.
 *
 * Note: This Operator is intended to support testing. Using this
 * operator on unbounded Observable sources may lead to memory exhaustion.
 */
let toList: RxObservable.t('a) => RxObservable.t(list('a));
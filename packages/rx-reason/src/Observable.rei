/**
 * A provider of push-based notifications.
 */;
type t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);

  /**
   * Returns a delayed subscription to the Observable with the
   * optional item and completion handlers.
   *
   * When the returned connect function is first invoked, it will
   * establish the subscription, returning a Disposable which can
   * be used to cancel the subscription. Subsequent calls to the
   * connect function will return the same Disposable instance,
   * unless the connection has been disposed, in which case a new
   * subscription will be created.
   */
  let publish:
    (
      ~onNext: 'a => unit=?,
      ~onComplete: option(exn) => unit=?,
      t('a),
      unit
    ) =>
    Disposable.t;

  /**
   * Returns a delayed subscription to the Observablew ith the supplied
   * item and completion handlers.
   *
   * Prefer using this publish variant when supplying both item and completion
   * handlers or to avoid optional argument boxing.
   */
  let publishTo:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a), unit) =>
    Disposable.t;

  /**
   * Subscribes to the Observable with the optional item and completion handlers.
   */
  let subscribe:
    (~onNext: 'a => unit=?, ~onComplete: option(exn) => unit=?, t('a)) =>
    Disposable.t;

  /**
   * Subscribes to the Observable with the supplied item and completion handlers.
   *
   * Prefer using this subscribe variant when supplying both item and completion
   * handlers or to avoid optional argument boxing.
   */
  let subscribeWith:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
    Disposable.t;
};

include S1 with type t('a) := t('a);

/**
 * Combines 2 source Observables, emitting items which aggregate the latest
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the
 * provided selector function.
 */
let combineLatest2: (~selector: ('a, 'b) => 'c, t('a), t('b)) => t('c);

/**
 * Combines 3 source Observables, emitting items which aggregate the latest
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the
 * provided selector function.
 */
let combineLatest3:
  (~selector: ('a, 'b, 'c) => 'd, t('a), t('b), t('c)) => t('d);

/**
 * Combines 4 source Observables, emitting items which aggregate the latest
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the
 * provided selector function.
 */
let combineLatest4:
  (~selector: ('a, 'b, 'c, 'd) => 'e, t('a), t('b), t('c), t('d)) => t('e);

/**
 * Combines 5 source Observables, emitting items which aggregate the latest
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the
 * provided selector function.
 */
let combineLatest5:
  (
    ~selector: ('a, 'b, 'c, 'd, 'e) => 'f,
    t('a),
    t('b),
    t('c),
    t('d),
    t('e)
  ) =>
  t('f);

/**
 * Combines 6 source Observables, emitting items which aggregate the latest
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the
 * provided selector function.
 */
let combineLatest6:
  (
    ~selector: ('a, 'b, 'c, 'd, 'e, 'f) => 'g,
    t('a),
    t('b),
    t('c),
    t('d),
    t('e),
    t('f)
  ) =>
  t('g);

/**
 * Combines 7 source Observables, emitting items which aggregate the latest
 * values emitted by each source Observable each time an item is emitted by
 * any of the source Observable. The combined value is computed using the
 * provided selector function.
 */
let combineLatest7:
  (
    ~selector: ('a, 'b, 'c, 'd, 'e, 'f, 'g) => 'h,
    t('a),
    t('b),
    t('c),
    t('d),
    t('e),
    t('f),
    t('g)
  ) =>
  t('h);

/**
 * Returns an Observable which concatenates elements of each provided Observable
 * into a single sequence of elements without interleaving them.
 */
let concat: (~scheduler: Scheduler.t=?, list(t('a))) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create:
  ((~onNext: 'a => unit, ~onComplete: option(exn) => unit) => Disposable.t) =>
  t('a);

let debounce: (Scheduler.t, t('a)) => t('a);

let defaultIfEmpty: ('a, t('a)) => t('a);

/**
 * Returns an Observable that calls an Observable factory to
 * create a new Observable for each subscription.
 */
let defer: (unit => t('a)) => t('a);

let distinctUntilChanged: (~equals: ('a, 'a) => bool=?, t('a)) => t('a);

/**
 * Returns an Observable that emits no items and immediately completes.
 */
let empty: (~scheduler: Scheduler.t=?, unit) => t('a);

let every: ('a => bool, t('a)) => t(bool);

let exhaust: t(t('a)) => t('a);

let find: ('a => bool, t('a)) => t('a);

let first: t('a) => t('a);

let firstOrNone: t('a) => t(option('a));

let ignoreElements: t('a) => t('a);

let isEmpty: t('a) => t(bool);

let keep: ('a => bool, t('a)) => t('a);

let last: t('a) => t('a);

let lastOrNone: t('a) => t(option('a));

/**
 * Returns an Observable that applies the Operator function to the
 * source Observable's notifications.
 */
let lift: (Operator.t('a, 'b), t('a)) => t('b);

let map: ('a => 'b, t('a)) => t('b);

let mapTo: ('b, t('a)) => t('b);

let materialize: t('a) => t(Notification.t('a));

let maybeFirst: t('a) => t('a);

let maybeLast: t('a) => t('a);

/**
 * Returns an Observable that merges items emitted by the source Observables,
 * interleaving the items emitted by each Observable.
 */
let merge: list(t('a)) => t('a);

/**
 * Returns an Observable the emits no values and never completes.
 */
let never: t('a);

let none: ('a => bool, t('a)) => t(bool);

let observe:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) => t('a);

let observeOn:
  (
    ~bufferStrategy: BufferStrategy.t=?,
    ~bufferSize: int=?,
    Scheduler.t,
    t('a)
  ) =>
  t('a);

/**
 * Returns an Observable which emits the specified notifications at the
 * specified absolutes times using the supplied scheduler.
 */
let ofAbsoluteTimeNotifications:
  (~scheduler: ClockScheduler.t, list((float, Notification.t('a)))) =>
  t('a);

/**
 * Returns an Observable that emits the given items and then completes.
 */
let ofList: (~scheduler: Scheduler.t=?, list('a)) => t('a);

/**
 * Returns an Observable which emits the specified notifications
 * using the supplied scheduler.
 */
let ofNotifications:
  (~scheduler: Scheduler.t=?, list(Notification.t('a))) => t('a);

/**
 * Returns an Observable which emits the specified notifications at the
 * specified relative times using the supplied scheduler.
 */
let ofRelativeTimeNotifications:
  (~scheduler: DelayScheduler.t, list((float, Notification.t('a)))) =>
  t('a);

/**
  * Returns an Observable that emits the given item and then completes.
  */
let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

let onComplete: (option(exn) => unit, t('a)) => t('a);

let onNext: ('a => unit, t('a)) => t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * calling the specified function when subscribed to, and
 * disposing the returned Disposable when disposed.
 */
let onSubscribe: (unit => Disposable.t, t('a)) => t('a);

let pipe2: (Operator.t('a, 'b), Operator.t('b, 'c), t('a)) => t('c);

let pipe3:
  (Operator.t('a, 'b), Operator.t('b, 'c), Operator.t('c, 'd), t('a)) =>
  t('d);

let pipe4:
  (
    Operator.t('a, 'b),
    Operator.t('b, 'c),
    Operator.t('c, 'd),
    Operator.t('d, 'e),
    t('a)
  ) =>
  t('e);

/**
 * Returns an Observable that completes with the specified exception
 * when subscribed to.
 */
let raise: (~scheduler: Scheduler.t=?, exn) => t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * resubscribing to it if it completes and
 * the predicate returns true for that specific.
 */
let repeat: (~predicate: unit => bool=?, t('a)) => t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * resubscribing to it if it completes with an exception and
 * the predicate returns true for that specific exception.
 */
let retry: (~predicate: exn => bool=?, t('a)) => t('a);

let scan: (('acc, 'a) => 'acc, 'acc, t('a)) => t('acc);

let some: ('a => bool, t('a)) => t(bool);

/**
 * Returns an Observable that emits the specified items before it
 * begins to emit items emitted by the source Observable.
 */
let startWithList: (~scheduler: Scheduler.t=?, list('a), t('a)) => t('a);

/**
 * Returns an Observable that emits a specified item before it
 * begins to emit items emitted by the source Observable.
 */
let startWithValue: (~scheduler: Scheduler.t=?, 'a, t('a)) => t('a);

/**
 * Returns an Observable which subscribes to the provided source Observable
 * on the specified scheduler.
 */
let subscribeOn: (Scheduler.t, t('a)) => t('a);

let switch_: t(t('a)) => t('a);

let synchronize: t('a) => t('a);

/*
let timeout: (Scheduler.t, t('a)) => t('a);*/

let toList: t('a) => t(list('a));

let withLatestFrom: (~selector: ('a, 'b) => 'c, t('b), t('a)) => t('c);
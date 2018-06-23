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

  let publish1:
    (
      ~onNext: ('ctx, 'a) => unit=?,
      ~onComplete: ('ctx, option(exn)) => unit=?,
      'ctx,
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

  let publishTo1:
    (
      ~onNext: ('ctx, 'a) => unit,
      ~onComplete: ('ctx, option(exn)) => unit,
      'ctx,
      t('a),
      unit
    ) =>
    Disposable.t;

  /**
   * Subscribes to the Observable with the optional item and completion handlers.
   */
  let subscribe:
    (~onNext: 'a => unit=?, ~onComplete: option(exn) => unit=?, t('a)) =>
    CompositeDisposable.t;

  let subscribe1:
    (
      ~onNext: ('ctx0, 'a) => unit=?,
      ~onComplete: ('ctx0, option(exn)) => unit=?,
      'ctx0,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe2:
    (
      ~onNext: ('ctx0, 'ctx1, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe3:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      'ctx2,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe4:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe5:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe6:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, option(exn)) =>
                   unit
                     =?,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe7:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'a) => unit=?,
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
                   unit
                     =?,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      'ctx6,
      t('a)
    ) =>
    CompositeDisposable.t;

  /**
   * Subscribes to the Observable with the supplied item and completion handlers.
   *
   * Prefer using this subscribe variant when supplying both item and completion
   * handlers or to avoid optional argument boxing.
   */
  let subscribeWith:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
    CompositeDisposable.t;

  let subscribeWith1:
    (
      ~onNext: ('ctx0, 'a) => unit,
      ~onComplete: ('ctx0, option(exn)) => unit,
      'ctx0,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith2:
    (
      ~onNext: ('ctx0, 'ctx1, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit,
      'ctx0,
      'ctx1,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith3:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
      'ctx0,
      'ctx1,
      'ctx2,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith4:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith5:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith6:
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
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith7:
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
      t('a)
    ) =>
    CompositeDisposable.t;
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
let create: (Subscriber.t('a) => unit) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create1: (('ctx0, Subscriber.t('a)) => unit, 'ctx0) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create2:
  (('ctx0, 'ctx1, Subscriber.t('a)) => unit, 'ctx0, 'ctx1) => t('a);

/**
 * Returns an Observable from the specified subscribe function.
 */
let create3:
  (('ctx0, 'ctx1, 'ctx3, Subscriber.t('a)) => unit, 'ctx0, 'ctx1, 'ctx3) =>
  t('a);

/**
 * Returns an Observable which drops items from the source that
 * are followed by another item within a debounce duration
 * determined by the scheduler.
 *
 * For instance, a DelayScheduler can be used debounce items every 5 ms:
 * ```re
 * someObservable
 * |> Observable.debounce(scheduleWithDelay(5.0);
 * ```
 */
let debounce: (Scheduler.t, t('a)) => t('a);

/**
 * Returns an Observable which emits a default value if the
 * source completes without producing any values.
 */
let defaultIfEmpty: ('a, t('a)) => t('a);

/**
 * Returns an Observable that calls an Observable factory to
 * create a new Observable for each subscription.
 */
let defer: (unit => t('a)) => t('a);

/**
 * Returns an Observable which emits items that are distinct from
 * their immediate predecessors based upon the provided
 * equality function. By default, reference equality is
 * used.
 */
let distinctUntilChanged: (~equals: ('a, 'a) => bool=?, t('a)) => t('a);

/**
 * Returns an Observable that emits no items and immediately completes.
 */
let empty: (~scheduler: SchedulerNew.t=?, unit) => t('a);

/**
 * Returns an Observable that emits a single true value if all
 * items emitted by the source satisfy the predicate.
 *
 * Note: If the source completes before emitting any values,
 * completes with true.
 */
let every: ('a => bool, t('a)) => t(bool);

/**
 * Returns an Observable that flattens Observable items,
 * dropping inner Observables until the current inner
 * Observable completes. Also see: switch_
 */
let exhaust: t(t('a)) => t('a);

/**
 * Returns an Observable which emits the first observed item
 * from the source which satisfies the predicate.
 */
let find: ('a => bool, t('a)) => t('a);

/**
 * Returns an Observable which emits the first observed item
 * from the source or completes with EmptyException.
 */
let first: t('a) => t('a);

/**
 * Returns an Observable which emits Some of the first observed
 * item from the source or emits None.
 */
let firstOrNone: t('a) => t(option('a));

/**
 * Returns an Observable which drops all items from the source.
 */
let ignoreElements: t('a) => t('a);

/**
 * Returns an Observable which emits true if the source emits
 * no items. Otherwise it emits false.
 */
let isEmpty: t('a) => t(bool);

/**
 * Returns an Observable which only emits items from the source
 * which satisfy the predicate.
 */
let keep: ('a => bool, t('a)) => t('a);

/**
 * Returns an Observable which emits the last observed item or
 * completes with EmptyException.
 */
let last: t('a) => t('a);

/**
 * Returns an Observable which emits Some of the last observed
 * item or emits None.
 */
let lastOrNone: t('a) => t(option('a));

/**
 * Returns an Observable that applies the Operator function to the
 * source Observable's notifications.
 */
let lift: (Operator.t('a, 'b), t('a)) => t('b);

/**
 * Returns an Observable which applies the specified function to
 * each item observed, emitting the result of the function application.
 */
let map: ('a => 'b, t('a)) => t('b);

/**
 * Returns an Observable which emits the specified value for
 * each item emitted by the source.
 */
let mapTo: ('b, t('a)) => t('b);

/**
 * Returns an Observable which emits all notifications
 * as Notification instances.
 */
let materialize: t('a) => t(Notification.t('a));

/**
 * Returns an Observable which emits the first observed item, if present, and
 * completes.
 */
let maybeFirst: t('a) => t('a);

/**
 * Returns an Observable which emits the last observed item, if present, and
 * completes.
 */
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

/**
 * Returns an Observable that emits a single true value if every
 * observed item fails to satisfy the predicate.
 *
 * Note: If the source completes before observing any values,
 * completes with true.
 */
let none: ('a => bool, t('a)) => t(bool);

/**
 * Returns an Observable which invokes side-effect functions on each
 * observed event and on completion.
 */
let observe:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) => t('a);

/**
 * Returns an Observable which emits notifications on the specified scheduler. The default
 * buffer strategy is to throw, and default buffer size is unbound.
 */
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

/**
 * Returns an Observable which invokes the side-effect function when it completes.
 */
let onComplete: (option(exn) => unit, t('a)) => t('a);

/**
 * Returns an Observable which invokes the side-effect function
 * with each observed item.
 */
let onNext: ('a => unit, t('a)) => t('a);

/**
 * Returns an Observable that mirrors the source Observable,
 * calling the specified function when subscribed to, and
 * disposing the returned Disposable when disposed.
 */
let onSubscribe: (unit => Disposable.t, t('a)) => t('a);

/**
 * Returns an Observable that completes with the specified exception
 * when subscribed to.
 */
let raise: (~scheduler: SchedulerNew.t=?, exn) => t('a);

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

/**
 * Returns an Observable which invokes the accumulator function
 * for each item observed from the source, emitting the intermediate
 * accumulated values. The first item emitted is the initial value.
 */
let scan: (('acc, 'a) => 'acc, 'acc, t('a)) => t('acc);

/**
 * Returns an Observable that emits a single true value if any
 * observed item satisfies the predicate.
 *
 * Note: If the source Observable complete before producing any values,
 * completes with false.
 */
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
let subscribeOn: (SchedulerNew.t, t('a)) => t('a);

/**
 * Returns an Observable that flattens Observable items,
 * unsubscribing from the observed inner Observable when the
 * next one is observed. Also see: exhaust.
 */
let switch_: t(t('a)) => t('a);

/**
 * Returns an Observable that utilizes locking, to ensure subscriber
 * callbacks are synchronized.
 *
 * Note: Only defined to support a future native ocaml version.
 */
let synchronize: t('a) => t('a);

/**
 * Returns an Observable which completes with a TimeoutException if
 * no notifications are observed before a duration determined
 * by the scheduler.
 *
 * For instance, a DelayScheduler can be used to timeout items every 5 ms:
 * ```re
 * someObservable
 * |> Observable..timeout(scheduleWithDelay(5.0));
 * ```
 */
let timeout: (SchedulerNew.t, t('a)) => t('a);

/**
 * Returns an Observable which collects all observed items in a list
 * and emits a single list of observed values in the observed
 * order when completed.
 *
 * Note: This Operator is intended to support testing. Using this
 * operator on unbounded Observable sources may lead to memory exhaustion.
 */
let toList: t('a) => t(list('a));

/**
 * Returns an Observable that combines each observed item from the source with
 * the last observed item from the other Observable using the selector function.
 *
 * Note: Observed items from the source are ignored, if the other Observable
 * has not yet produced any values.
 */
let withLatestFrom: (~selector: ('a, 'b) => 'c, t('b), t('a)) => t('c);
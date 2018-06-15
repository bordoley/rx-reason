/**
 * Common Operator functions.
 */

exception EmptyException;

exception TimeoutException;

/** Chains Operator functions together. For instance:
 * ```re
 * Observable.ofList([1, 2 , 3])
 * |> Observable.lift(
 *     Operators.(map(x => x + 1) >> first)
 *   );
 * ```
 * 
 */
let (>>): (Operator.t('a, 'b), Operator.t('b, 'c)) => Operator.t('a, 'c);

/**
 * Returns an Observer which drops observed items that are followed by another item 
 * within a debounce duration determined by the scheduler. 
 * 
 * For instance, a DelayScheduler can be used debounce items every 5 ms:
 * ```re
 * Observable.ofList([1, 2 , 3])
 * |> Observable.lift(
 *     Operators.debounce(scheduleWithDelay(~delay=5.0))
 *   );
 * ```
 */
let debounce: Scheduler.t => Operator.t('a, 'a);

/**
 * Returns an Observer which emits a default value if the
 * upstream Observer is completed without producing any values.
 */
let defaultIfEmpty: 'a => Operator.t('a, 'a);

/**
 * Returns an Observer which emits items that are distinct from 
 * their immediate predecessors based upon the provided
 * equality function. By default, reference equality is
 * used.
 */
let distinctUntilChanged:
  (~equals: ('a, 'a) => bool=?) => Operator.t('a, 'a);

/**
 * Returns an Observer that emits a single true value if every 
 * item observed by the upstream observer satisfies
 * the predicate.
 * 
 * Note: If the Observer is completed before observing any values
 * it will complete with true.
 */
let every: ('a => bool) => Operator.t('a, bool);

/** 
 * Returns an Observer that flattens Observable items,
 * dropping observed inner Observables until the current inner
 * Observable has completed. Also see: switch_
 */
let exhaust: Operator.t(Observable.t('a), 'a);

/** 
 * Returns an Observer which emits the first observed item
 * which satisfies the predicate.
 */
let find: ('a => bool) => Operator.t('a, 'a);

/** 
 * Returns an Observer which emits the first observed item or
 * completes with EmptyException.
 */
let first: Operator.t('a, 'a);

/** 
 * Returns an Observer which emits Some of the first observed 
 * item or emits None.
 */
let firstOrNone: Operator.t('a, option('a));

/**
 * Returns an Observer which drops all observed items.
 */
let ignoreElements: Operator.t('a, 'a);

/**
 * Returns an Observer which emits true if it observes
 * no items. Otherwise it emits false.
 */
let isEmpty: Operator.t('a, bool);

/**
 * Returns an Observer which only emits items observed which satisfy the predicate.
 */
let keep: ('a => bool) => Operator.t('a, 'a);

/** 
 * Returns an Observer which emits the last observed item or
 * completes with EmptyException.
 */
let last: Operator.t('a, 'a);

/** 
 * Returns an Observer which emits Some of the last observed 
 * item or emits None.
 */
let lastOrNone: Operator.t('a, option('a));

/**
 * Returns an Observer which applies the specified function to
 * each item observed, emitting the
 * result of the function application.
 */
let map: ('a => 'b) => Operator.t('a, 'b);

/**
 * Returns an Observer which emits the specified value for
 * each item observed.
 */
let mapTo: 'b => Operator.t('a, 'b);

/**
 * Returns an Observer which emits all observed notifications observed
 * as Notification instances.
 */
let materialize: Operator.t('a, Notification.t('a));

/**
 * Returns an Observer which emits the first observed item, if present, and
 * completes.
 */
let maybeFirst: Operator.t('a, 'a);

/**
 * Returns an Observer which emits the last observed item, if present, and
 * completes.
 */
let maybeLast: Operator.t('a, 'a);

/**
 * Returns an Observer that emits a single true value if every 
 * observed item fails to satisfy the predicate.
 * 
 * Note: If the Observer is completed before observing any values
 * it will complete with true.
 */
let none: ('a => bool) => Operator.t('a, bool);

/**
 * Returns an Observer which invokes side-effect functions on each
 * observed event and on completion.
 */
let observe:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) =>
  Operator.t('a, 'a);

/**
 * Returns an Observer which emits notifications on the specified scheduler. The default
 * buffer strategy is to throw, and default buffer size is unbound.
 */
let observeOn:
  (~bufferStrategy: BufferStrategy.t=?, ~bufferSize: int=?, Scheduler.t) =>
  Operator.t('a, 'a);

/**
 * Returns an Observer which invokes the side-effect function when it completes.
 */
let onComplete: (option(exn) => unit) => Operator.t('a, 'a);

/**
 * Returns an Observer which invokes the side-effect function with each observed item.
 */
let onNext: ('a => unit) => Operator.t('a, 'a);

/**
 * Returns an Observer which invokes the accumulator function for each item observed,
 * emitting the intermediate produced values. The first item emitted by the observer
 * is the initial value.
 */
let scan: (('acc, 'a) => 'acc, 'acc) => Operator.t('a, 'acc);

/**
 * Returns an Observer that emits a single true value if any 
 * observed item satisfies the predicate.
 * 
 * Note: If the Observer is completed before observing any values
 * it will complete with false.
 */
let some: ('a => bool) => Operator.t('a, bool);

/** 
 * Returns an Observer that flattens Observable items,
 * unsubscribing from the observed inner Observable when the
 * next one is observed. Also see: exhaust.
 */
let switch_: Operator.t(Observable.t('a), 'a);

/**
 * Returns an Observer that utilizes locking, such that it can be 
 * called from any operating system thread.
 * 
 * Note: Only defined to support a future native ocaml version. 
 */
let synchronize: Operator.t('a, 'a);

/**
 * Returns an Observer which completes with a TimeoutException if
 * no Notifications are observed before a duration determined
 * by the scheduler. 
 * 
 * For instance, a DelayScheduler can be used timeout items every 5 ms:
 * ```re
 * someObservable
 * |> Observable.lift(
 *     Operators.timeout(scheduleWithDelay(~delay=5.0))
 *   );
 * ```
 */
let timeout: Scheduler.t => Operator.t('a, 'a);

/**
 * Returns an Observer which collects all observed items in a list
 * and emits a single list of observed values in observed
 * order when completed.
 * 
 * Note: This Operator is intended to support testing. Using this
 * operator on unbounded Observable sources may lead to memory exhaustion.
 */
let toList: Operator.t('a, list('a));

/**
 * Returns an Observer that combines each observed item with 
 * the last observed item from the other Observable using the selector function.
 * 
 * Note: Observed items are ignored, if the other Observable has not yet produced
 * any values.
 */
let withLatestFrom:
  (~selector: ('a, 'b) => 'c, Observable.t('b)) => Operator.t('a, 'c);
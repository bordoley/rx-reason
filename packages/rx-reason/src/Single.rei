/**
 * A notification provider which is guaranteed to complete with either
 * a single value or an exception. Conceptually similar to a Promise or Task.
 */;
type t('a);

/** Cast to Observable.t. */
let asObservable: t('a) => Observable.t('a);

/**
 * Returns a Single from the specified subscribe function.
 */
let create:
  ((~onSuccess: 'a => unit, ~onError: exn => unit) => Disposable.t) => t('a);

/**
 * Returns a Single that calls a Single factory to
 * create a new Single for each subscription.
 */
let defer: (unit => t('a)) => t('a);

/*
 * Returns a Single that completes with true if all
 * items emitted by the source Observable satisfies the predicate.
 *
 * If the source Observable emits no values, it will complete with true.
 */
let every: ('a => bool, Observable.t('a)) => t(bool);

/**
 * Returns a Single which emits the first item emitted by the
 * soure Observable which satisfies the predicate.
 *
 * If the Observable emits no items or no items satisfy the
 * predicte, the Single complete with an EmptyException error.
 */
let find: ('a => bool, Observable.t('a)) => t('a);

/**
 * Returns a Single that succeeds with the first item
 * emitted by the source Observable or fails with
 * EmptyException if the source emits no values.
 */
let first: Observable.t('a) => t('a);

/**
 * Returns a single that apply the supplied function
 * to the result of supplied Single, when it completes successfully,
 * flattening the result.
 */
let flatMap: ('a => t('b), t('a)) => t('b);

/**
 * Returns a Single that succeeds with the last item
 * emitted by the source Observable or fails with
 * EmptyException if the source emits no values.
 */
let last: Observable.t('a) => t('a);

/**
 * Returns a Single that applies the Operator function to the
 * source Observable's notifications, succeeding with the first item
 * emitted by the lifted Observable or failing with
 * EmptyException if the lifted Observable emits no values.
 */
let liftFirst: (Operator.t('a, 'b), Observable.t('a)) => t('b);

/**
 * Returns a Single that applies the Operator function to the
 * source Observable's notifications, succeeding with the last item
 * emitted by the lifted Observable or failing with
 * EmptyException if the lifted Observable emits no values.
 */
let liftLast: (Operator.t('a, 'b), Observable.t('a)) => t('b);

/**
 * Returns a Single which completes with the result of applying
 * the specified function to the result of supplied Single
 * when it completes successfully.
 */
let map: ('a => 'b, t('a)) => t('b);

let mapTo: ('b, t('a)) => t('b);

/*
 * Returns a Single that completes with true if no
 * items emitted by the source Observable satisfies the predicate.
 *
 * If the source Observable emits no values, it will complete with true.
 */
let none: ('a => bool, Observable.t('a)) => t(bool);

/**
 * Returns a Single that always succeeds with the given item
 * when subscribed to.
 */
let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

/**
 * Returns a Single that fails with the given exception
 * when subscribed to.
 */
let raise: (~scheduler: Scheduler.t=?, exn) => t('a);

/**
 * Returns a Single which invokes the accumulator function to each
 * item emitted by the source Observable, completing with the final
 * value accumulated value.
 */
let reduce: (('acc, 'a) => 'acc, 'acc, Observable.t('a)) => t('acc);

/*
 * Returns a Single that completes with true if any
 * item emitted by the source Observable satisfies the predicate.
 *
 * If the source Observable emits no values, it will complete with false.
 */
let some: ('a => bool, Observable.t('a)) => t(bool);

/**
 * Subscribes to the Single with the optional success and error handlers.
 */
let subscribe:
  (~onSuccess: 'a => unit=?, ~onError: exn => unit=?, t('a)) => Disposable.t;

/**
 * Subscribes to the Single with the provided success and error handlers.
 *
 * Prefer using this subscribe variant when supplying both success and error
 * handlers or to avoid optional argument boxing.
 */
let subscribeWith:
  (~onSuccess: 'a => unit, ~onError: exn => unit, t('a)) => Disposable.t;
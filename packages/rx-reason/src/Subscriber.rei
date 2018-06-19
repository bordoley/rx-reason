/**
 * Represents the underlying subscription to an Observable and it's callbacks.
 * Subscribers are a specialized type, primarily used in the implementation of
 * Operators. Normal use cases, such as subscribing to an Observable, should use
 * callback functions instead.
 */;
type t('a);

type subscriber('a) = t('a);

module type S1 = {
  type t('a);

  include CompositeDisposable.S1 with type t('a) := t('a);
  include Observer.S1 with type t('a) := t('a);

  /** Cast to Subscriber.t. */
  let asSubscriber: t('a) => subscriber('a);
};

include S1 with type t('a) := t('a);

/** Construct a new Subscriber with the provided callbacks. */
let create: (~onNext: 'a => unit, ~onComplete: option(exn) => unit) => t('a);

/**
 * Construct a new Subscriber with the provided callbacks which automatically disposes
 * itself when completed.
 * */
let createAutoDisposing:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) => t('a);

/** Construct a new Subscriber with the provided callbacks that delegates its disposal to another Subscriber. */
let delegate:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('b)) => t('a);

/**
 * A disposed Subscriber instance.
 */
let disposed: t('a);

/** Returns an onComplete function which forwards the notifcation to the provided Subscriber. */
let forwardOnComplete: (t('a), option(exn)) => unit;

/** Returns an onNext function which forwards the notifcation to the provided Subscriber. */
let forwardOnNext: (t('a), 'a) => unit;
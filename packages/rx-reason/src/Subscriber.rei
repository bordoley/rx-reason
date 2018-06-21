/**
 * Represents the underlying subscription to an Observable and it's callbacks.
 * Subscribers are a specialized type, primarily used in the implementation of
 * Operators. Normal use cases, such as subscribing to an Observable, should use
 * callback functions instead.
 */;
type t('a);

include CompositeDisposable.S1 with type t('a) := t('a);
include Observer.S1 with type t('a) := t('a);

/**
 * Construct a new Subscriber with the provided callbacks which automatically disposes
 * itself when completed.
 * */
let createAutoDisposing:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) => t('a);

/** Construct a new Subscriber with the provided callbacks that delegates its disposal to another Subscriber. */
let delegate:
  (
    ~onNext: (t('b), 'a) => unit,
    ~onComplete: (t('b), option(exn)) => unit,
    t('b)
  ) =>
  t('a);

let delegate1:
  (
    ~onNext: ('ctx0, t('b), 'a) => unit,
    ~onComplete: ('ctx0, t('b), option(exn)) => unit,
    'ctx0,
    t('b)
  ) =>
  t('a);

let delegate2:
  (
    ~onNext: ('ctx0, 'ctx1, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    t('b)
  ) =>
  t('a);

let delegate3:
  (
    ~onNext: ('ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
    'ctx0,
    'ctx1,
    'ctx2,
    t('b)
  ) =>
  t('a);

let delegateOnComplete: (t('a), option(exn)) => unit;
let delegateOnComplete1: ('ctx0, t('a), option(exn)) => unit;
let delegateOnComplete2: ('ctx0, 'ctx1, t('a), option(exn)) => unit;
let delegateOnComplete3: ('ctx0, 'ctx1, 'ctx2, t('a), option(exn)) => unit;

let delegateOnNext: (t('a), 'a) => unit;
let delegateOnNext1: ('ctx0, t('a), 'a) => unit;
let delegateOnNext2: ('ctx0, 'ctx1, t('a), 'a) => unit;
let delegateOnNext3: ('ctx0, 'ctx1, 'ctx2, t('a), 'a) => unit;

/**
 * A disposed Subscriber instance.
 */
let disposed: t('a);

/** Returns true if the Observer has been completed or disposed. */
let isStopped: t('a) => bool;
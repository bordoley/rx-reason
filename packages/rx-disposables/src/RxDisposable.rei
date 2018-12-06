/**
 * A disposable resource providing a mechanism for releasing unmanaged resources.
 */;

/** The Disposable type */
type t;
type disposable = t;

/**
 * Module type signature for a Disposable type with parametric type arity of 0.
 */
module type S = {
  include RxDisposableLike.S;

  /** Cast to Disposable.t. */
  let asDisposable: t => disposable;
};

/**
 * Module type signature for a Disposable type with parametric type arity of 1.
 */
module type S1 = {
  include RxDisposableLike.S1;

  /** Cast to Disposable.t. */
  let asDisposable: t('a) => disposable;
};

/**
 * Module type signature for a Disposable type with parametric type arity of 2.
 */
module type S2 = {
  include RxDisposableLike.S2;

  /** Cast to Disposable.t. */
  let asDisposable: t('a, 'b) => disposable;
};

include RxDisposableLike.S with type t := t;

/**
 * Constructs a Disposable instance which disposes the
 * supplied disposables when disposed.
 */
let compose: list(t) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided function exactly once when disposed.
 */
let create: (unit => unit) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create1: ('a => unit, 'a) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create2: (('a, 'b) => unit, 'a, 'b) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create3: (('a, 'b, 'c) => unit, 'a, 'b, 'c) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create4: (('a, 'b, 'c, 'd) => unit, 'a, 'b, 'c, 'd) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create5: (('a, 'b, 'c, 'd, 'e) => unit, 'a, 'b, 'c, 'd, 'e) => t;

/**
 * A disposed Disposable instance.
 */
let disposed: t;

/**
 * Contructs a new, non-disposed Disposable instance.
 */
let empty: unit => t;
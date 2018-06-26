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
  include DisposableLike.S;

  /** Cast to Disposable.t. */
  let asDisposable: t => disposable;
};

/**
 * Module type signature for a Disposable type with parametric type arity of 1.
 */
module type S1 = {
  include DisposableLike.S1;

  /** Cast to Disposable.t. */
  let asDisposable: t('a) => disposable;
};

include DisposableLike.S with type t := t;

/**
 * Constructs a Disposable instance which disposes the
 * supplied disposables when disposed.
 */
let compose: list(t) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided function exactly once when disposed.
 */
let create: TeardownLogic.t => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create1: (TeardownLogic.t1('a), 'a) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create2: (TeardownLogic.t2('a, 'b), 'a, 'b) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create3: (TeardownLogic.t3('a, 'b, 'c), 'a, 'b, 'c) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create4: (TeardownLogic.t4('a, 'b, 'c, 'd), 'a, 'b, 'c, 'd) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create5: (TeardownLogic.t5('a, 'b, 'c, 'd, 'e), 'a, 'b, 'c, 'd, 'e) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create6:
  (TeardownLogic.t6('a, 'b, 'c, 'd, 'e, 'f), 'a, 'b, 'c, 'd, 'e, 'f) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided teardown function exactly once when disposed.
 */
let create7:
  (
    TeardownLogic.t7('a, 'b, 'c, 'd, 'e, 'f, 'g),
    'a,
    'b,
    'c,
    'd,
    'e,
    'f,
    'g
  ) =>
  t;

/**
 * A disposed Disposable instance.
 */
let disposed: t;

/**
 * Contructs a new, non-disposed Disposable instance.
 */
let empty: unit => t;
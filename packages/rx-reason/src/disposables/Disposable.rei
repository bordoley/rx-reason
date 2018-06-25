/**
 * A disposable resource providing a mechanism for releasing unmanaged resources.
 */;

type t;

type disposable = t;

module type S = {
  type t;
  
  /** Cast to Disposable.t. */
  let asDisposable: t => disposable;

  /** Dispose the resource. The operation should be idempotent. */
  let dispose: t => unit;
  
  /** Returns true if this resource has been disposed. */
  let isDisposed: t => bool;
  
  /** Raises a DisposedException if the the Disposable is disposed. */
  let raiseIfDisposed: t => unit;
};

module type S1 = {
  type t('a);
  
  /** Cast to Disposable.t. */
  let asDisposable: t('a) => disposable;
  
  /** Dispose the resource. The operation should be idempotent. */
  let dispose: t('a) => unit;
  
  /** Returns true if this resource has been disposed. */
  let isDisposed: t('a) => bool;
  
  /** Raises a DisposedException if the the Disposable is disposed. */
  let raiseIfDisposed: t('a) => unit;
};

include S with type t := t;

/**
 * Constructs a Disposable instance which disposes the
 * supplied disposables when disposed.
 */
let compose: list(t) => t;

/**
 * Constructs a Disposable instance which executes
 * the provided function exactly once when disposed.
 */
let create: (TeardownLogic.t) => t;

let create1: (TeardownLogic.t1('a), 'a) => t;

let create2: (TeardownLogic.t2('a, 'b), 'a, 'b) => t;

let create3: (TeardownLogic.t3('a, 'b, 'c), 'a, 'b, 'c) => t;

let create4: (TeardownLogic.t4('a, 'b, 'c, 'd), 'a, 'b, 'c, 'd) => t;

let create5: (TeardownLogic.t5('a, 'b, 'c, 'd, 'e), 'a, 'b, 'c, 'd, 'e) => t;

let create6: (TeardownLogic.t6('a, 'b, 'c, 'd, 'e, 'f), 'a, 'b, 'c, 'd, 'e, 'f) => t;

let create7: (TeardownLogic.t7('a, 'b, 'c, 'd, 'e, 'f, 'g), 'a, 'b, 'c, 'd, 'e, 'f, 'g) => t;

/**
 * A disposed Disposable instance.
 */
let disposed: t;

/**
 * Contructs a new, non-disposed Disposable instance.
 */
let empty: unit => t;
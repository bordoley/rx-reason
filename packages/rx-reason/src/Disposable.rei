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

/**
 * A disposed Disposable instance.
 */
let disposed: t;

/**
 * Contructs a new, non-disposed Disposable instance.
 */
let empty: unit => t;
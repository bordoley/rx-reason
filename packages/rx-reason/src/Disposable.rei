/**
 * A disposable resource providing a mechanism for releasing unmanaged resources.
 */;

type t;
type disposable = t;

exception DisposedException;

module type S = {
  type t;

  /** Dispose the resource. The operation should be idempotent. */
  let dispose: t => unit;

  /** Returns true if this resource has been disposed. */
  let isDisposed: t => bool;
  
  /** Raises a DisposedException if the the Disposable is disposed. */
  let raiseIfDisposed: t => unit;
  
  /** Cast to Disposable.t. */
  let toDisposable: t => disposable;
};

module type S1 = {
  type t('a);

  /** Dispose the resource. The operation should be idempotent. */
  let dispose: t('a) => unit;
  
  /** Returns true if this resource has been disposed. */
  let isDisposed: t('a) => bool;
  
  /** Raises a DisposedException if the the Disposable is disposed. */
  let raiseIfDisposed: t('a) => unit;
  
  /** Cast to Disposable.t. */
  let toDisposable: t('a) => disposable;
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
let create: (unit => unit) => t;

/**
 * A disposed Disposable instance.
 */
let disposed: t;

/**
 * Contructs a new, non-disposed Disposable instance.
 */
let empty: unit => t;
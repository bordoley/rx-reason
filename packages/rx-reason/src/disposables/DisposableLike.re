/**
 * Module type signature for a DisposableLike type with parametric type arity of 0.
 */
module type S = {
  type t;

  /** Dispose the resource. The operation must be idempotent. */
  let dispose: t => unit;

  /** Returns true if this resource has been disposed. */
  let isDisposed: t => bool;

  /** Raises a DisposedException if the the Disposable is disposed. */
  let raiseIfDisposed: t => unit;
};

/**
 * Module type signature for a DisposableLike type with parametric type arity of 1.
 */
module type S1 = {
  type t('a);

  /** Dispose the resource. The operation must be idempotent. */
  let dispose: t('a) => unit;

  /** Returns true if this resource has been disposed. */
  let isDisposed: t('a) => bool;

  /** Raises a DisposedException if the the Disposable is disposed. */
  let raiseIfDisposed: t('a) => unit;
};
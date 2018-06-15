/**
 * A Disposable container that allows atomically updating/replacing the contained
 * Disposable with another Disposable, disposing the old one when updating plus
 * handling the disposition when the container itself is disposed.
 */;

type t;

include Disposable.S with type t := t;

/** Constructs a SerialDisposable containing a disposed disposable. */
let create: unit => t;

/** Returns the currently contained Disposable */
let get: t => Disposable.t;

/** Atomically: set the next disposable on this container and dispose the previous
 *  one (if any) or dispose next if the container has been disposed
 */
let set: (Disposable.t, t) => unit;
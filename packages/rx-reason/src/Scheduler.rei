/**
 * Schedules work to be performed. The exact behavior
 * of when and how work is performed is implementation
 * specific.
 */;
type t = (unit => Disposable.t) => Disposable.t;

/** Schedules work immediately on the current stack frame. */
let immediate: t;
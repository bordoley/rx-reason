/**
 * Scheduler implementations that schedule work on the javascript
 * event loop.
 */;

/**
 * A Scheduler instance that schedules work on the javascript event loop.
 *
 * The underlying implementation uses a resolved javascript Promise to schedule
 * a queued microtask to be processed on the next tick of the queue.
 */
let scheduler: RxReason.Scheduler.t;

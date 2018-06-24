/**
 * Scheduler implementations that schedule work on the javascript
 * event loop.
 */;

/** A ClockScheduler instance that schedules work on the javascript event loop. */
let clockScheduler: RxReason.ClockScheduler.t;

/**
 * A Scheduler instance that schedules work on the javascript event loop.
 * 
 * The underlying implementation uses a resolved javascript Promise to schedule
 * a queued microtask to be processed on the next tick of the queue.
 */
let schedulerNew: RxReason.SchedulerNew.t;

/**
 * A DelayScheduler instance that schedules work on the javascript event loop.
 * 
 * The underlying implementation uses the global setTimeout function and as such
 * is limited by the known setTimeout limitations.
 */
let scheduleWithDelay: RxReason.DelayScheduler.t;

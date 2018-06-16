/**
 * A Scheduler that schedules work items using virtual time in ms.
 */;

type t;

/** Advances the scheduler by 1 virtual ms. */
let advance: t => unit;

/** Casts to DelayScheduler.t */
let asDelayScheduler: t => DelayScheduler.t;

/** Constructs a new VirtualTimeScheduler */
let create: unit => t;

/** Returns the current time in milliseconds. */
let now: t => float;

/** Runs the VirtualTimeScheduler until it has no more work to execute. */
let run: t => unit;

/** Returns a new ClockScheduler instance that schedules work on the specified VirtualTimeScheduler. */
let toClockScheduler: t => ClockScheduler.t;

/** Returns a new Scheduler instance that schedules work on the specified VirtualTimeScheduler.  */
let toScheduler: t => Scheduler.t;
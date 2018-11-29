/**
 * A Scheduler that schedules work items using virtual time in ms.
 */;

type t;

/** Returns a new Scheduler instance that schedules work on the specified VirtualTimeScheduler.  */
let asScheduler: t => RxScheduler.t;

/** Advances the scheduler by 1 virtual ms. */
let advance: t => unit;

/** Constructs a new VirtualTimeScheduler. */
let create: unit => t;

/** Runs the VirtualTimeScheduler until it has no more work to execute. */
let run: t => unit;
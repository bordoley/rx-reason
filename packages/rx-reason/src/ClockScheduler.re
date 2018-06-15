/**
 * A Scheduler that can be used to schedule work either immediately or
 * in the future, and also provides a mechanism to retrieve the current 
 * time.
 */
type t = {
  /** Returns the current time in milliseconds. */
  now: unit => float,
  
  /** 
   * Schedules worked to be performed immediately. The underlying 
   * implementation determines the definition of immediately.
   * For instance, an event loop may schedule work to be performed
   * on the next clock tick.
   */
  schedule: Scheduler.t,

  /** 
   * Schedules work to be performed at some number of milliseconds 
   * in the future. 
   */
  scheduleWithDelay: DelayScheduler.t,
};
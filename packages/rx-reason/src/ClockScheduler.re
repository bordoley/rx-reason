type t = {
  schedule: Scheduler.t,
  scheduleWithDelay: DelayScheduler.t,
  now: unit => float,
};
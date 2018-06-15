type t = {
  schedule: Scheduler.t,
  scheduleWithDelay: DelayScheduler.t,
  getCurrentTime: unit => float,
};
type t = {
  now: unit => float,
  schedule: DelayScheduler.t,
};

let create = (~now: unit => float, ~schedule: DelayScheduler.t) : t => {
  now,
  schedule,
};

let now = (scheduler: t) : float => scheduler.now();

let toDelayScheduler = (delay: float, {schedule}: t) : Scheduler.t =>
  schedule(delay);
type t = {
  now: unit => int,
  schedule: DelayScheduler.t,
};

let create = (~now: unit => int, ~schedule: DelayScheduler.t) : t => {
  now,
  schedule,
};

let now = (scheduler: t) : int => scheduler.now();

let toDelayScheduler = (delay: int, {schedule}: t) : Scheduler.t =>
  schedule(delay);
type t = {
  now: unit => float,
  schedule: DelayScheduler.t,
};

let create = (~now: unit => float, ~schedule: DelayScheduler.t) : t => {
  now,
  schedule,
};

let now = (scheduler: t) : float => scheduler.now();

let toDelayScheduler = ({schedule}: t) : DelayScheduler.t =>
  schedule;
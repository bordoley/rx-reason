type t = (float) => Scheduler.t;
let toSchedulerWithDelay=(delay:float, scheduler: t): Scheduler.t => 
  scheduler(delay);
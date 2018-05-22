type t = (int) => Scheduler.t;
let toSchedulerWithDelay=(delay:int, scheduler: t): Scheduler.t => 
  scheduler(delay);
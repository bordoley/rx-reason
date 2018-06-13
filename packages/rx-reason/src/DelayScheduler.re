type t = (~delay: float) => Scheduler.t;

module type S = {
  include Scheduler.S;
  let scheduleWithDelay: (~delay: float) => Scheduler.t;
};
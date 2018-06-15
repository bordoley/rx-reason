type t;

let advance: t => unit;
let asDelayScheduler: t => DelayScheduler.t;
let create: unit => t;
let now: t => float;
let run: t => unit;
let toClockScheduler: t => ClockScheduler.t;
let toScheduler: t => Scheduler.t;
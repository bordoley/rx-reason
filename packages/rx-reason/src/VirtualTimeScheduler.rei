type t;

let create: unit => t;
let getCurrentTime: t => int;
let run: t => unit;
let toClockScheduler: t => ClockScheduler.t;
let toDelayScheduler: t => DelayScheduler.t;
let toScheduler: t => Scheduler.t;
type t;

let advance: t => unit;
let create: unit => t;
let getCurrentTime: t => float;
let run: t => unit;

let toClockScheduler: t => ClockScheduler.t;
let toDelayScheduler: t => DelayScheduler.t;
let toScheduler: t => Scheduler.t;
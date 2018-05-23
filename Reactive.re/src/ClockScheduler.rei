type t;

let create: (~now: unit => float, ~schedule: DelayScheduler.t) => t;

let now: t => float;

let toDelayScheduler: t => DelayScheduler.t;
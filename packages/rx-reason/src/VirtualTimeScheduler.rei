type t;

let advance: t => unit;
let create: unit => t;
let getCurrentTime: t => float;
let run: t => unit;
let toDelayScheduler: t => DelayScheduler.t;
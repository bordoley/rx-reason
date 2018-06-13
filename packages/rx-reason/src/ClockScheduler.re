type t = ((float, DelayScheduler.t) => Disposable.t) => Disposable.t;

let create = (~getCurrentTime, scheduler) : t => f => f(getCurrentTime(), scheduler);
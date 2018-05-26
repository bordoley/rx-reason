type t = (~delay:float) => Scheduler.t;

let default: t = (~delay as _: float, _:  unit => Disposable.t): Disposable.t =>
  failwith("Not Implemented");
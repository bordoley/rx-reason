type t = (unit => Disposable.t) => Disposable.t;

let immediate: t = (work: unit => Disposable.t) => (work(): Disposable.t);

let schedule = (work: unit => Disposable.t, scheduler: t) : Disposable.t =>
  scheduler(work);
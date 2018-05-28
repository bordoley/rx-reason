type t = (unit => Disposable.t) => Disposable.t;

let immediate: t = (work: unit => Disposable.t) => (work(): Disposable.t);

let currentThread: t = TrampolineScheduler.currentThreadInstance;
type t =
  ((float, DelayScheduler.t) => Disposable.t) => Disposable.t;
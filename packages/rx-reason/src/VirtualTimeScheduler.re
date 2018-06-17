type t = {
  currentTime: ref(int),
  disposable: Disposable.t,
  /* FIXME: Don't use Belt directly. Instead add a Multimap to platform */
  timeQueue: Belt.MutableMap.Int.t(MutableQueue.t(unit => unit)),
  scheduler: DelayScheduler.t,
};

let create = () => {
  let currentTime = ref(-1);
  let timeQueue = Belt.MutableMap.Int.make();
  let disposable =
    Disposable.create(() => timeQueue |> Belt.MutableMap.Int.clear);
  let scheduler = (delay, work) => {
    Disposable.raiseIfDisposed(disposable);
    let currentTime = currentTime^;
    let scheduleTime = currentTime + int_of_float(delay);

    if (scheduleTime >= currentTime) {
      let disposable = SerialDisposable.create();
      let executeWork = () =>
        if (! SerialDisposable.isDisposed(disposable)) {
          disposable |> SerialDisposable.set(work());
        };

      switch (Belt.MutableMap.Int.get(timeQueue, scheduleTime)) {
      | Some(queue) => queue |> MutableQueue.enqueue(executeWork)
      | None =>
        let queue = MutableQueue.create();
        queue |> MutableQueue.enqueue(executeWork);
        Belt.MutableMap.Int.set(timeQueue, scheduleTime, queue);
      };

      disposable |> SerialDisposable.asDisposable;
    } else {
      Disposable.disposed;
    };
  };

  {currentTime, disposable, timeQueue, scheduler};
};

let advance = ({disposable, timeQueue} as vts: t) => {
  let currentTime = vts.currentTime^;

  switch (Belt.MutableMap.Int.get(timeQueue, currentTime)) {
  | None => ()
  | Some(queue) =>
    while (! MutableQueue.isEmpty(queue)
           && ! Disposable.isDisposed(disposable)) {
      let work = MutableQueue.dequeue(queue);
      work();
    }
  };

  Belt.MutableMap.Int.remove(timeQueue, currentTime);
  incr(vts.currentTime);
};

let asDelayScheduler = ({scheduler}) => scheduler;

let run = ({disposable, timeQueue} as vts: t) => {
  Disposable.raiseIfDisposed(disposable);

  let break = ref(false);
  while (! break^) {
    advance(vts);
    break :=
      Belt.MutableMap.Int.isEmpty(timeQueue)
      || Disposable.isDisposed(disposable);
  };
  disposable |> Disposable.dispose;
};

let now = ({currentTime}: t) => float_of_int(currentTime^);

let toScheduler = vts => asDelayScheduler(vts, 0.0);

let toClockScheduler = (vts: t) : ClockScheduler.t => {
  now: () => now(vts),
  schedule: toScheduler(vts),
  scheduleWithDelay: asDelayScheduler(vts),
};
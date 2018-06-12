type t = {
  currentTime: ref(int),
  disposable: Disposable.t,
  /* FIXME: Don't use Belt directly. Instead add a Multimap to platform */
  timeQueue: Belt.MutableMap.Int.t(MutableQueue.t(unit => unit)),
  scheduler: DelayScheduler.t,
};

let create = () => {
  let currentTime = ref(0);
  let timeQueue = Belt.MutableMap.Int.make();
  let disposable =
    Disposable.create(() => timeQueue |> Belt.MutableMap.Int.clear);
  let scheduler = (~delay, work) => {
    Disposable.raiseIfDisposed(disposable);
    let currentTime = currentTime^;
    let scheduleTime = currentTime + int_of_float(delay);

    if (scheduleTime >= currentTime) {
      let disposable = AssignableDisposable.create();
      let executeWork = () =>
        if (! AssignableDisposable.isDisposed(disposable)) {
          disposable |> AssignableDisposable.set(work());
        };

      switch (Belt.MutableMap.Int.get(timeQueue, scheduleTime)) {
      | Some(queue) => queue |> MutableQueue.enqueue(executeWork)
      | None =>
        let queue = MutableQueue.create();
        queue |> MutableQueue.enqueue(executeWork);
        Belt.MutableMap.Int.set(timeQueue, scheduleTime, queue);
      };

      disposable |> AssignableDisposable.toDisposable;
    } else {
      Disposable.disposed;
    };
  };

  {currentTime, disposable, timeQueue, scheduler};
};

let run = ({disposable, timeQueue} as vts: t) => {
  Disposable.raiseIfDisposed(disposable);

  let break = ref(false);
  while (! break^) {
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
    vts.currentTime := vts.currentTime^ + 1;
    break :=
      Belt.MutableMap.Int.isEmpty(timeQueue)
      || Disposable.isDisposed(disposable);
  };
  disposable |> Disposable.dispose;
};

let getCurrentTime = ({currentTime, disposable}: t) => {
  Disposable.raiseIfDisposed(disposable);
  currentTime^;
};

let toClockScheduler = ({currentTime, disposable, scheduler}) : ClockScheduler.t => {
  Disposable.raiseIfDisposed(disposable);
  f => {
    Disposable.raiseIfDisposed(disposable);
    f(float_of_int(currentTime^), scheduler);
  };
};

let toDelayScheduler = ({disposable, scheduler}) => {
  Disposable.raiseIfDisposed(disposable);
  scheduler;
};

let toScheduler = ({disposable,scheduler}) => {
  Disposable.raiseIfDisposed(disposable);
  scheduler(~delay=0.0);
};
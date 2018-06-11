type t = {
  currentTime: ref(int),
  /* FIXME: Don't use Belt directly. Instead add a Multimap to platform */
  timeQueue: Belt.MutableMap.Int.t(MutableQueue.t(unit => unit)),
  scheduler: ClockScheduler.t,
};

let create = () => {
  let currentTime = ref(0);
  let timeQueue = Belt.MutableMap.Int.make();
  let schedule = (~delay, work) => {
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
  let scheduler = f => f(float_of_int(currentTime^), schedule);

  {currentTime, timeQueue, scheduler};
};

let run = (vts: t) => {
  let break = ref(false);

  while (! break^) {
    let currentTime = vts.currentTime^;

    switch (Belt.MutableMap.Int.get(vts.timeQueue, currentTime)) {
    | None => ()
    | Some(queue) =>
      while (! MutableQueue.isEmpty(queue)) {
        let work = MutableQueue.dequeue(queue);
        work();
      }
    };

    Belt.MutableMap.Int.remove(vts.timeQueue, currentTime);
    vts.currentTime := vts.currentTime^ + 1;
    break := Belt.MutableMap.Int.isEmpty(vts.timeQueue);
  };
};
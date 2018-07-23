type t = {
  currentTime: ref(int),
  disposable: Disposable.t,
  /* FIXME: Don't use Belt directly. Instead add a Multimap to platform */
  timeQueue: Belt.MutableMap.Int.t(MutableQueue.t(unit => unit)),
  scheduler: Scheduler.t,
};

let asScheduler = ({scheduler}) => scheduler;

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

let create = () => {
  let currentTime = ref(0);
  let timeQueue = Belt.MutableMap.Int.make();
  let disposable = Disposable.create1(Belt.MutableMap.Int.clear, timeQueue);

  let schedule = (delay, work) => {
    let currentTime = currentTime^;
    let scheduleTime = currentTime + int_of_float(delay);

    if (scheduleTime >= currentTime) {
      switch (Belt.MutableMap.Int.get(timeQueue, scheduleTime)) {
      | Some(queue) => queue |> MutableQueue.enqueue(work)
      | None =>
        let queue = MutableQueue.create();
        queue |> MutableQueue.enqueue(work);
        Belt.MutableMap.Int.set(timeQueue, scheduleTime, queue);
      };
    };
  };

  let scheduler: Scheduler.t = {
    now: () => currentTime^ |> float_of_int,
    scheduleAfter: (~delay, f, state) => {
      let work = () => f(state);
      schedule(delay, work);
      Disposable.disposed;
    },
    schedulePeriodic: (~delay, f, state) => {
      let disposable = Disposable.empty();

      let rec work = () => {
        f(state);
        if (! Disposable.isDisposed(disposable)) {
          schedule(delay, work);
        };
      };

      schedule(delay, work);
      disposable;
    },
  };

  {currentTime, disposable, timeQueue, scheduler};
};

let now = ({currentTime}: t) => float_of_int(currentTime^);

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
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
    while ({
      let isDisposed = Disposable.isDisposed(disposable);
      let head = MutableQueue.peek(queue);
      (head !== None) && !isDisposed;
    }) {
      switch (MutableQueue.dequeue(queue)) {
      | Some(work) => work()
      | None => ()
      };
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

  let now = () => currentTime^ |> float_of_int;

  let scheduler: Scheduler.t = {
    now,
    schedule: (~delay=?, continuation) => {
      let disposable = Disposable.empty();

      let rec work = (continuation, ()) =>
        if (! Disposable.isDisposed(disposable)) {
          continuation(~now, ~shouldYield=Functions.alwaysFalse1)
          |> Scheduler.Result.continueWith(scheduleWork);
        }
      and scheduleWork = (~delay=0.0, continuation) =>
        if (! Disposable.isDisposed(disposable)) {
          schedule(delay, work(continuation));
        };

      scheduleWork(~delay?, continuation);
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
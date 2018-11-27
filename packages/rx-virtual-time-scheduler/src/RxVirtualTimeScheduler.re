type t = {
  currentTime: ref(int),
  disposable: RxDisposable.t,
  /* FIXME: Don't use Belt directly. Instead add a Multimap to platform */
  timeQueue: Belt.MutableMap.Int.t(RxMutableQueue.t(unit => unit)),
  scheduler: RxScheduler.t,
};

let asScheduler = ({scheduler}) => scheduler;

let advance = ({disposable, timeQueue} as vts: t) => {
  let currentTime = vts.currentTime^;

  switch (Belt.MutableMap.Int.get(timeQueue, currentTime)) {
  | None => ()
  | Some(queue) =>
    while ({
      let isDisposed = RxDisposable.isDisposed(disposable);
      let head = RxMutableQueue.peek(queue);
      (head !== None) && !isDisposed;
    }) {
      switch (RxMutableQueue.dequeue(queue)) {
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
  let disposable = RxDisposable.create1(Belt.MutableMap.Int.clear, timeQueue);

  let schedule = (delay, work) => {
    let currentTime = currentTime^;
    let scheduleTime = currentTime + int_of_float(delay);

    if (scheduleTime >= currentTime) {
      switch (Belt.MutableMap.Int.get(timeQueue, scheduleTime)) {
      | Some(queue) => queue |> RxMutableQueue.enqueue(work)
      | None =>
        let queue = RxMutableQueue.create();
        queue |> RxMutableQueue.enqueue(work);
        Belt.MutableMap.Int.set(timeQueue, scheduleTime, queue);
      };
    };
  };

  let now = () => currentTime^ |> float_of_int;

  let scheduler: RxScheduler.t = {
    now,
    schedule: (~delay=?, continuation) => {
      let disposable = RxDisposable.empty();

      let rec work = (continuation, ()) =>
        if (! RxDisposable.isDisposed(disposable)) {
          continuation(~now, ~shouldYield=RxFunctions.alwaysFalse1)
          |> RxScheduler.Result.continueWith(scheduleWork);
        }
      and scheduleWork = (~delay=0.0, continuation) =>
        if (! RxDisposable.isDisposed(disposable)) {
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
  RxDisposable.raiseIfDisposed(disposable);

  let break = ref(false);
  while (! break^) {
    advance(vts);
    break :=
      Belt.MutableMap.Int.isEmpty(timeQueue)
      || RxDisposable.isDisposed(disposable);
  };
  disposable |> RxDisposable.dispose;
};
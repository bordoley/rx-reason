let queue: array(unit => RxReason.Disposable.t) = [||];
let id = ref(None);

let drainStep = (disposable, ()) => {
  let result = Js.Array.shift(queue);
  let isDisposed = disposable |> RxReason.AssignableDisposable.isDisposed;

  if (!isDisposed) {
    switch (result) {
    | Some(work) => disposable |> RxReason.AssignableDisposable.set(work());
    | None => ()
    }  
  };

  switch(id^) {
  | Some(intervalId) when Js.Array.length(queue) === 0 =>
      Js.Global.clearInterval(intervalId);
      id := None;
  | _ => ()
  }
};

let eventloop: RxReason.Scheduler.t = (work: unit => RxReason.Disposable.t) => {
  queue |> Js.Array.push(work) |> ignore;

  let disposable = RxReason.AssignableDisposable.create();

  if (Js.Array.length(queue) === 1) {
    let intervalId = Js.Global.setInterval(drainStep(disposable), 0);
    id := Some(intervalId);
  };
  disposable |> RxReason.AssignableDisposable.toDisposable;
};  
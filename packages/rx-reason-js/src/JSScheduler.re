let eventloop: RxReason.Scheduler.t = (work: unit => RxReason.Disposable.t) => {
  let disposable = RxReason.AssignableDisposable.create();
  
  let id = Js.Global.setInterval(() => {
    /* Dispose the interval subscription */
    disposable |> RxReason.AssignableDisposable.set(RxReason.Disposable.disposed);
    
    let isDisposed = disposable |> RxReason.AssignableDisposable.isDisposed;
    if (!isDisposed) {

      let workDisposable = work();
      disposable |> RxReason.AssignableDisposable.set(workDisposable);
    };
  }, 0);

  let intervalDisposable = RxReason.Disposable.create(() => Js.Global.clearInterval(id));
  disposable |> RxReason.AssignableDisposable.set(intervalDisposable);
  disposable |> RxReason.AssignableDisposable.toDisposable;
};  
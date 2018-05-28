let create = () => {
  let queue = MutableQueue.create();

  let run = () =>
    while (Js.Array.length(queue) > 0) {
      let (disposable, work) = MutableQueue.peek(queue);
      if (! AssignableDisposable.isDisposed(disposable)) {
        disposable |> AssignableDisposable.set(work());
      };
      MutableQueue.dequeue(queue) |> ignore;
    };

  (work: unit => Disposable.t) => {
    let disposable = AssignableDisposable.create();
    let shouldRun = MutableQueue.isEmpty(queue);
    queue |> MutableQueue.enqueue((disposable, work));
    if (shouldRun) {
      run();
    };
    disposable |> AssignableDisposable.toDisposable;
  };
};

let currentThreadInstance = create();
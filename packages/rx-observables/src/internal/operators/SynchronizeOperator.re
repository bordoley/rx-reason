let operator =  {
  let onNext = (gate, delegate, next) => {
    RxLock.acquire(gate);
    delegate |> RxSubscriber.next(next);
    RxLock.release(gate);
  };

  let onComplete = (gate, delegate, exn) => {
    RxLock.acquire(gate);
    delegate |> RxSubscriber.complete(~exn?);
    RxLock.release(gate);
  };

  subscriber => {
    let gate = RxLock.create();
    subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, gate);
  };
};

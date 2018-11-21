let operator =  {
  let onNext = (gate, delegate, next) => {
    Lock.acquire(gate);
    delegate |> Subscriber.next(next);
    Lock.release(gate);
  };

  let onComplete = (gate, delegate, exn) => {
    Lock.acquire(gate);
    delegate |> Subscriber.complete(~exn?);
    Lock.release(gate);
  };

  subscriber => {
    let gate = Lock.create();
    subscriber |> Subscriber.decorate1(~onNext, ~onComplete, gate);
  };
};

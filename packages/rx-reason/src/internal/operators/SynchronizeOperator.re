let operator = subscriber => {
  let gate = Lock.create();
  
  let onNext = next => {
    Lock.acquire(gate);
    subscriber |> Subscriber.next(next);
    Lock.release(gate);
  };

  let onComplete = exn => {
    Lock.acquire(gate);
    subscriber |> Subscriber.complete(~exn?);
    Lock.release(gate);
  };

  subscriber |> Subscriber.delegate(~onNext, ~onComplete);
};

let lift = observable => observable |> ObservableSource.lift(operator);
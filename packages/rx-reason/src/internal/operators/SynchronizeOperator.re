let operator = observer => {
  let gate = Lock.create();
  
  let onNext = next => {
    Lock.acquire(gate);
    observer |> Observer.next(next);
    Lock.release(gate);
  };

  let onComplete = exn => {
    Lock.acquire(gate);
    observer |> Observer.complete(~exn?);
    Lock.release(gate);
  };

  observer |> Observer.delegate(~onNext, ~onComplete);
};

let lift = observable => observable |> ObservableSource.lift(operator);
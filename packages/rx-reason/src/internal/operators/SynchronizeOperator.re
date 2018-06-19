let operator = observer => {
  let gate = Lock.create();
  Observer.delegate(
    ~onComplete=
      exn => {
        Lock.acquire(gate);
        observer |> Observer.complete(~exn?);
        Lock.release(gate);
      },
    ~onNext=
      next => {
        Lock.acquire(gate);
        observer |> Observer.next(next);
        Lock.release(gate);
      },
      observer,
  );
};

let lift = observable =>
  observable |> ObservableSource.lift(operator);
let operator = (default, subscriber) => {
  let isEmpty = ref(true);

  let onNext = next => {
    subscriber |> Subscriber.next(next);
    Volatile.write(false, isEmpty);
  };

  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(EmptyException.Exn)
      | None =>
        if (Volatile.read(isEmpty)) {
          subscriber |> Subscriber.next(default);
        };
        None;
      | Some(_) => exn
      };
    subscriber |> Subscriber.complete(~exn?);
  };
  
  subscriber |> Subscriber.delegate(~onNext, ~onComplete);
};

let lift = (default, observable) =>
  observable |> ObservableSource.lift(operator(default));
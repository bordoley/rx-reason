let operator = subscriber => {
  let last = MutableOption.create();

  let onNext = next => MutableOption.set(next, last);
  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(_) => exn
      | None =>
        if (MutableOption.isEmpty(last)) {
          Some(EmptyException.Exn);
        } else {
          let lastValue = MutableOption.get(last);
          subscriber |> Subscriber.next(lastValue);
          None;
        }
      };
    subscriber |> Subscriber.complete(~exn?);
  };
  
  subscriber
  |> Subscriber.delegate(~onNext, ~onComplete)
  |> Subscriber.addTeardown(() => MutableOption.unset(last));
};

let lift = observable => observable |> ObservableSource.lift(operator);
let operator = {
  let onNext = (last, _, next) => MutableOption.set(next, last);

  let onComplete = (last, delegate, exn) => {
    let exn =
      switch (exn) {
      | Some(_) => exn
      | None =>
        if (MutableOption.isEmpty(last)) {
          Some(EmptyException.Exn);
        } else {
          let lastValue = MutableOption.get(last);
          delegate |> Subscriber.next(lastValue);
          None;
        }
      };
    delegate |> Subscriber.complete(~exn?);
  };

  subscriber => {
    let last = MutableOption.create();
    subscriber
    |> Subscriber.delegate1(~onNext, ~onComplete, last)
    |> Subscriber.addTeardown1(MutableOption.unset, last);
  };
};

let lift = observable => observable |> ObservableSource.lift(operator);
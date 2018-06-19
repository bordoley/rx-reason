let operator = observer => {
  let last = MutableOption.create();
  Observer.delegate(
    ~onNext=next => MutableOption.set(next, last),
    ~onComplete=
      exn => {
        let exn =
          switch (exn) {
          | Some(_) => exn
          | None =>
            if (MutableOption.isEmpty(last)) {
              Some(EmptyException.exn);
            } else {
              let lastValue = MutableOption.get(last);
              observer |> Observer.next(lastValue);
              None;
            }
          };
        observer |> Observer.complete(~exn?);
      },
    observer,
  )
  |> Observer.addTeardown(() => MutableOption.unset(last));
};

let lift = observable => observable |> ObservableSource.lift(operator);
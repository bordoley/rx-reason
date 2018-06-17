let operator = observer => {
  let last = MutableOption.create();
  Observer.create(
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
    ~onDispose=
      () => {
        MutableOption.unset(last);
        observer |> Observer.dispose;
      },
  );
};

let lift = observable => observable |> ObservableSource.lift(operator);

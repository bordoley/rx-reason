let operator = (default, observer) => {
  let isEmpty = ref(true);
  Observer.create(
    ~onNext=
      next => {
        observer |> Observer.next(next);
        Volatile.write(false, isEmpty);
      },
    ~onComplete=
      exn => {
        let exn =
          switch (exn) {
          | Some(Exceptions.EmptyException)
          | None =>
            if (Volatile.read(isEmpty)) {
              observer |> Observer.next(default);
            };
            None;
          | Some(_) => exn
          };
        observer |> Observer.complete(~exn?);
      },
    ~onDispose=Observer.forwardOnDispose(observer),
  );
};

let lift = (default, observable) =>
  observable |> ObservableSource.lift(operator(default));
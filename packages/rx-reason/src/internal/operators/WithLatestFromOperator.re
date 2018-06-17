let operator = (~selector, other, observer) => {
  let otherLatest = MutableOption.create();

  let withLatestObserver = ref(Observer.disposed);
  let otherSubscription = ref(Disposable.disposed);

  withLatestObserver :=
    Observer.create(
      ~onNext=
        Functions.earlyReturnsUnit1(next =>
          if (MutableOption.isNotEmpty(otherLatest)) {
            let latest = otherLatest |> MutableOption.get;
            let nextWithLatest =
              try (selector(next, latest)) {
              | exn =>
                withLatestObserver^ |> Observer.complete(~exn);
                Functions.returnUnit();
              };
            observer |> Observer.next(nextWithLatest);
          }
        ),
      ~onComplete=
        exn => {
          observer |> Observer.complete(~exn?);
          otherSubscription^ |> Disposable.dispose;
        },
      ~onDispose=
        () => {
          otherSubscription^ |> Disposable.dispose;
          observer |> Observer.dispose;
        },
    );

  otherSubscription :=
    ObservableSource.subscribeWith(
      ~onNext=next => otherLatest |> MutableOption.set(next),
      ~onComplete=
        exn =>
          switch (exn) {
          | Some(_) => withLatestObserver^ |> Observer.complete(~exn?)
          | _ => ()
          },
      other,
    );

  withLatestObserver^;
};

let lift = (~selector, other, source) =>
  source |> ObservableSource.lift(operator(~selector, other));
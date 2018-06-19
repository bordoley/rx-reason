let operator = (~selector, other, subscriber) => {
  let otherLatest = MutableOption.create();
  let withLatestSubscriber = ref(Subscriber.disposed);
  let otherSubscription = ref(Disposable.disposed);

  let onNext =
    Functions.earlyReturnsUnit1(next =>
      if (MutableOption.isNotEmpty(otherLatest)) {
        let latest = otherLatest |> MutableOption.get;
        let nextWithLatest =
          try (selector(next, latest)) {
          | exn =>
            withLatestSubscriber^ |> Subscriber.complete(~exn);
            Functions.returnUnit();
          };
        subscriber |> Subscriber.next(nextWithLatest);
      }
    );

  let onComplete = exn => {
    subscriber |> Subscriber.complete(~exn?);
    otherSubscription^ |> Disposable.dispose;
  };

  withLatestSubscriber := subscriber |> Subscriber.delegate(~onNext, ~onComplete);

  otherSubscription :=
    ObservableSource.subscribeWith(
      ~onNext=next => otherLatest |> MutableOption.set(next),
      ~onComplete=
        exn =>
          switch (exn) {
          | Some(_) => withLatestSubscriber^ |> Subscriber.complete(~exn?)
          | _ => ()
          },
      other,
    )
    |> CompositeDisposable.asDisposable;

  withLatestSubscriber^ |> Subscriber.addDisposable(otherSubscription^);
};

let lift = (~selector, other, source) =>
  source |> ObservableSource.lift(operator(~selector, other));
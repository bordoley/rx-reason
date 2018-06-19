let operator = (~onNext, ~onComplete, subscriber) => {
  let observeSubscriber = ref(Subscriber.disposed);
  
  let onNext =
    Functions.earlyReturnsUnit1(next => {
      try (onNext(next)) {
      | exn =>
        observeSubscriber^ |> Subscriber.complete(~exn);
        Functions.returnUnit();
      };
      subscriber |> Subscriber.next(next);
    });

  let onComplete = exn => {
    let exn =
      try (
        {
          onComplete(exn);
          exn;
        }
      ) {
      | exn => Some(exn)
      };
    subscriber |> Subscriber.complete(~exn?);
  };

  observeSubscriber := subscriber |> Subscriber.delegate(~onNext, ~onComplete);
  observeSubscriber^;
};

let lift = (~onNext, ~onComplete, observable) =>
  observable |> ObservableSource.lift(operator(~onNext, ~onComplete));
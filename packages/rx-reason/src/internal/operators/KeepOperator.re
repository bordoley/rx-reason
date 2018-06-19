let operator = (predicate, subscriber) => {
  let keepSubscriber = ref(Subscriber.disposed);

  let onNext =
    Functions.earlyReturnsUnit1(next => {
      let shouldKeep =
        try (predicate(next)) {
        | exn =>
          keepSubscriber^ |> Subscriber.complete(~exn);
          Functions.returnUnit();
        };
      if (shouldKeep) {
        subscriber |> Subscriber.next(next);
      };
    });

  keepSubscriber :=
    subscriber
    |> Subscriber.delegate(
         ~onNext,
         ~onComplete=Subscriber.forwardOnComplete(subscriber),
       );
  keepSubscriber^;
};

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));
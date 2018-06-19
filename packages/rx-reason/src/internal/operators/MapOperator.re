let operator = (mapper, subscriber) => {
  let mapSubscriber = ref(Subscriber.disposed);
  let onNext =
    Functions.earlyReturnsUnit1(next => {
      let mapped =
        try (mapper(next)) {
        | exn =>
          mapSubscriber^ |> Subscriber.complete(~exn);
          Functions.returnUnit();
        };
      subscriber |> Subscriber.next(mapped);
    });
  mapSubscriber :=
    subscriber
    |> Subscriber.delegate(
         ~onNext,
         ~onComplete=Subscriber.forwardOnComplete(subscriber),
       );
  mapSubscriber^;
};

let lift = (mapper, observable) =>
  observable |> ObservableSource.lift(operator(mapper));
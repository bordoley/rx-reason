type context('a) = {
  predicate: 'a => bool,
  mutable self: RxSubscriber.t('a),
};

let operator = {
  let onNext = {
    let impl = ({predicate, self}, subscriber, next) => {
      let shouldKeep =
        try (predicate(next)) {
        | exn =>
          self |> RxSubscriber.complete(~exn);
          RxFunctions.returnUnit();
        };
      if (shouldKeep) {
        subscriber |> RxSubscriber.next(next);
      };
    };

    (ctx, subscriber, next) =>
      RxFunctions.earlyReturnsUnit3(impl, ctx, subscriber, next);
  };

  (predicate, subscriber) => {
    let context = {predicate, self: RxSubscriber.disposed};

    context.self =
      subscriber
      |> RxSubscriber.decorate1(
           ~onNext,
           ~onComplete=RxSubscriber.forwardOnComplete1,
           context,
         );

    context.self;
  };
};
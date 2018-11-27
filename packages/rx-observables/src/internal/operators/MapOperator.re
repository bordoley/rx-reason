type context('a, 'b) = {
  mapper: 'a => 'b,
  mutable self: RxSubscriber.t('a),
};

let operator = {
  let onNext = {
    let impl = ({mapper, self}, delegate, next) => {
      let mapped =
        try (mapper(next)) {
        | exn =>
          self |> RxSubscriber.complete(~exn);
          RxFunctions.returnUnit();
        };
      delegate |> RxSubscriber.next(mapped);
    };

    (ctx, delegate, next) =>
      RxFunctions.earlyReturnsUnit3(impl, ctx, delegate, next);
  };

  (mapper, subscriber) => {
    let context = {mapper, self: RxSubscriber.disposed};

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
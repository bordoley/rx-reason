type context('a, 'b) = {
  mapper: 'a => 'b,
  mutable self: Subscriber.t('a),
};

let operator = {
  let onNext = {
    let impl = ({mapper, self}, delegate, next) => {
      let mapped =
        try (mapper(next)) {
        | exn =>
          self |> Subscriber.complete(~exn);
          Functions.returnUnit();
        };
      delegate |> Subscriber.next(mapped);
    };

    (ctx, delegate, next) =>
      Functions.earlyReturnsUnit3(impl, ctx, delegate, next);
  };

  (mapper, subscriber) => {
    let context = {mapper, self: Subscriber.disposed};

    context.self =
      subscriber
      |> Subscriber.delegate(
           ~onNext,
           ~onComplete=Subscriber.forwardOnComplete,
           context,
         );

    context.self;
  };
};

let lift = (mapper, observable) =>
  observable |> ObservableSource.lift(operator(mapper));
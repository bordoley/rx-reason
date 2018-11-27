type context('a) = {
  onNext: 'a => unit,
  onComplete: option(exn) => unit,
  mutable self: RxSubscriber.t('a),
};

let operator = {
  let onNext = {
    let impl = ({onNext, self}, subscriber, next) => {
      try (onNext(next)) {
      | exn =>
        self |> RxSubscriber.complete(~exn);
        RxFunctions.returnUnit();
      };
      subscriber |> RxSubscriber.next(next);
    };

    (ctx, subscriber, next) =>
      RxFunctions.earlyReturnsUnit3(impl, ctx, subscriber, next);
  };

  let onComplete = ({onComplete}, subscriber, exn) => {
    let exn =
      try (
        {
          onComplete(exn);
          exn;
        }
      ) {
      | exn => Some(exn)
      };
    subscriber |> RxSubscriber.complete(~exn?);
  };

  (~onNext as observeOnNext, ~onComplete as observeOnComplete, subscriber) => {
    let context = {
      onNext: observeOnNext,
      onComplete: observeOnComplete,
      self: RxSubscriber.disposed,
    };
    context.self =
      subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, context);
    context.self;
  };
};
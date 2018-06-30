type context('a) = {
  onNext: 'a => unit,
  onComplete: option(exn) => unit,
  mutable self: Subscriber.t('a),
};

let operator = {
  let onNext = {
    let impl = ({onNext, self}, subscriber, next) => {
      try (onNext(next)) {
      | exn =>
        self |> Subscriber.complete(~exn);
        Functions.returnUnit();
      };
      subscriber |> Subscriber.next(next);
    };

    (ctx, subscriber, next) =>
      Functions.earlyReturnsUnit3(impl, ctx, subscriber, next);
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
    subscriber |> Subscriber.complete(~exn?);
  };

  (~onNext as observeOnNext, ~onComplete as observeOnComplete, subscriber) => {
    let context = {
      onNext: observeOnNext,
      onComplete: observeOnComplete,
      self: Subscriber.disposed,
    };
    context.self =
      subscriber |> Subscriber.decorate1(~onNext, ~onComplete, context);
    context.self;
  };
};

let lift = (~onNext, ~onComplete, observable) =>
  observable |> ObservableSource.lift(operator(~onNext, ~onComplete));
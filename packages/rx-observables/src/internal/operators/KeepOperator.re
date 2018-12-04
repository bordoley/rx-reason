let create = {
  let onNext = (predicate, subscriber, next) => {
    let shouldKeep = predicate(next);
    if (shouldKeep) {
      subscriber |> RxSubscriber.next(next);
    };
  };
  predicate =>
    RxSubscriber.decorate1(
      ~onNext,
      ~onComplete=SubscriberForward.onComplete1,
      predicate,
    );
};

let create1 = {
  let onNext = (predicate, ctx0, subscriber, next) => {
    let shouldKeep = predicate(ctx0, next);
    if (shouldKeep) {
      subscriber |> RxSubscriber.next(next);
    };
  };
  (predicate, ctx0) =>
    RxSubscriber.decorate2(
      ~onNext,
      ~onComplete=SubscriberForward.onComplete2,
      predicate,
      ctx0,
    );
};

let create2 = {
  let onNext = (predicate, ctx0, ctx1, subscriber, next) => {
    let shouldKeep = predicate(ctx0, ctx1, next);
    if (shouldKeep) {
      subscriber |> RxSubscriber.next(next);
    };
  };
  (predicate, ctx0, ctx1) =>
    RxSubscriber.decorate3(
      ~onNext,
      ~onComplete=SubscriberForward.onComplete3,
      predicate,
      ctx0,
      ctx1,
    );
};
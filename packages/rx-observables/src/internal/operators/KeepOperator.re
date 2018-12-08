let create = {
  let onNext = (predicate, subscriber, next) => {
    let shouldKeep = predicate(next);
    if (shouldKeep) {
      subscriber |> RxSubscriber.next(next);
    };
  };
  predicate => RxSubscriber.decorateOnNext1(onNext, predicate);
};

let create1 = {
  let onNext = (predicate, ctx0, subscriber, next) => {
    let shouldKeep = predicate(ctx0, next);
    if (shouldKeep) {
      subscriber |> RxSubscriber.next(next);
    };
  };
  (predicate, ctx0) =>
    RxSubscriber.decorateOnNext2(onNext, predicate, ctx0);
};

let create2 = {
  let onNext = (predicate, ctx0, ctx1, subscriber, next) => {
    let shouldKeep = predicate(ctx0, ctx1, next);
    if (shouldKeep) {
      subscriber |> RxSubscriber.next(next);
    };
  };
  (predicate, ctx0, ctx1) =>
    RxSubscriber.decorateOnNext3(onNext, predicate, ctx0, ctx1);
};
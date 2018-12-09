let create = predicate => {
  let onNext =
    (. predicate, subscriber, next) => {
      let shouldKeep = predicate(next);
      if (shouldKeep) {
        subscriber |> RxSubscriber.next(next);
      };
    };
  RxSubscriber.decorateOnNext1(onNext, predicate);
};

let create1 = (predicate, ctx0) => {
  let onNext =
    (. predicate, ctx0, subscriber, next) => {
      let shouldKeep = predicate(ctx0, next);
      if (shouldKeep) {
        subscriber |> RxSubscriber.next(next);
      };
    };
  RxSubscriber.decorateOnNext2(onNext, predicate, ctx0);
};

let create2 = (predicate, ctx0, ctx1) => {
  let onNext =
    (. predicate, ctx0, ctx1, subscriber, next) => {
      let shouldKeep = predicate(ctx0, ctx1, next);
      if (shouldKeep) {
        subscriber |> RxSubscriber.next(next);
      };
    };
  RxSubscriber.decorateOnNext3(onNext, predicate, ctx0, ctx1);
};

let create3 = (predicate, ctx0, ctx1, ctx2) => {
  let onNext =
    (. predicate, ctx0, ctx1, ctx2, subscriber, next) => {
      let shouldKeep = predicate(ctx0, ctx1, ctx2, next);
      if (shouldKeep) {
        subscriber |> RxSubscriber.next(next);
      };
    };
  RxSubscriber.decorateOnNext4(onNext, predicate, ctx0, ctx1, ctx2);
};
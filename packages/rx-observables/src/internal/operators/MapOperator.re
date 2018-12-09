let create = mapper => {
  let onNext =
    (. mapper, delegate, next) => {
      let mapped = mapper(next);
      delegate |> RxSubscriber.next(mapped);
    };

  RxSubscriber.decorateOnNext1(onNext, mapper);
};

let create1 = (mapper, ctx0) => {
  let onNext =
    (. ctx0, mapper, delegate, next) => {
      let mapped = mapper(ctx0, next);
      delegate |> RxSubscriber.next(mapped);
    };
  RxSubscriber.decorateOnNext2(onNext, ctx0, mapper);
};

let create2 = (mapper, ctx0, ctx1) => {
  let onNext =
    (. ctx0, ctx1, mapper, delegate, next) => {
      let mapped = mapper(ctx0, ctx1, next);
      delegate |> RxSubscriber.next(mapped);
    };
  RxSubscriber.decorateOnNext3(onNext, ctx0, ctx1, mapper);
};

let create3 = (mapper, ctx0, ctx1, ctx2) => {
  let onNext =
    (. ctx0, ctx1, ctx2, mapper, delegate, next) => {
      let mapped = mapper(ctx0, ctx1, ctx2, next);
      delegate |> RxSubscriber.next(mapped);
    };
  RxSubscriber.decorateOnNext4(onNext, ctx0, ctx1, ctx2, mapper);
};
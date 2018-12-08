let create = {
  let onNext = (mapper, delegate, next) => {
    let mapped = mapper(next);
    delegate |> RxSubscriber.next(mapped);
  };

  mapper => RxSubscriber.decorateOnNext1(~onNext, mapper);
};

let create1 = {
  let onNext = (ctx0, mapper, delegate, next) => {
    let mapped = mapper(ctx0, next);
    delegate |> RxSubscriber.next(mapped);
  };

  (mapper, ctx0) => RxSubscriber.decorateOnNext2(~onNext, ctx0, mapper);
};

let create2 = {
  let onNext = (ctx0, ctx1, mapper, delegate, next) => {
    let mapped = mapper(ctx0, ctx1, next);
    delegate |> RxSubscriber.next(mapped);
  };

  (mapper, ctx0, ctx1) =>
    RxSubscriber.decorateOnNext3(~onNext, ctx0, ctx1, mapper);
};
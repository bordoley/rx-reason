let create = {
  let onNext = (mapper, delegate, next) => {
    let mapped = mapper(next);
    delegate |> RxSubscriber.next(mapped);
  };

  mapper =>
    RxSubscriber.decorate1(
      ~onNext,
      ~onComplete=SubscriberForward.onComplete1,
      mapper,
    );
};

let create1 = {
  let onNext = (ctx0, mapper, delegate, next) => {
    let mapped = mapper(ctx0, next);
    delegate |> RxSubscriber.next(mapped);
  };
  
  (mapper, ctx0) =>
    RxSubscriber.decorate2(
      ~onNext,
      ~onComplete=SubscriberForward.onComplete2,
      ctx0,
      mapper,
    );
};
let create = {
  let onNext_ = (onNext, _, subscriber, next) => {
    onNext(next);
    subscriber |> RxSubscriber.next(next);
  };
  let onComplete_ = (_, onComplete, subscriber, exn) => {
    onComplete(exn);
    subscriber |> RxSubscriber.complete(~exn?);
  };

  (~onNext, ~onComplete, subscriber) =>
    subscriber
    |> RxSubscriber.decorate2(
         ~onNext=onNext_,
         ~onComplete=onComplete_,
         onNext,
         onComplete,
       );
};

let create1 = {
  let onNext_ = (onNext, _, ctx0, subscriber, next) => {
    onNext(ctx0, next);
    subscriber |> RxSubscriber.next(next);
  };
  let onComplete_ = (_, onComplete, ctx0, subscriber, exn) => {
    onComplete(ctx0, exn);
    subscriber |> RxSubscriber.complete(~exn?);
  };

  (~onNext, ~onComplete, ctx0, subscriber) =>
    subscriber
    |> RxSubscriber.decorate3(
         ~onNext=onNext_,
         ~onComplete=onComplete_,
         onNext,
         onComplete,
         ctx0,
       );
};

let create2 = {
  let onNext_ = (onNext, _, ctx0, ctx1, subscriber, next) => {
    onNext(ctx0, ctx1, next);
    subscriber |> RxSubscriber.next(next);
  };
  let onComplete_ = (_, onComplete, ctx0, ctx1, subscriber, exn) => {
    onComplete(ctx0, ctx1, exn);
    subscriber |> RxSubscriber.complete(~exn?);
  };

  (~onNext, ~onComplete, ctx0, ctx1, subscriber) =>
    subscriber
    |> RxSubscriber.decorate4(
         ~onNext=onNext_,
         ~onComplete=onComplete_,
         onNext,
         onComplete,
         ctx0,
         ctx1,
       );
};

let create3 = {
  let onNext_ = (onNext, _, ctx0, ctx1, ctx2, subscriber, next) => {
    onNext(ctx0, ctx1, ctx2, next);
    subscriber |> RxSubscriber.next(next);
  };
  let onComplete_ = (_, onComplete, ctx0, ctx1, ctx2, subscriber, exn) => {
    onComplete(ctx0, ctx1, ctx2, exn);
    subscriber |> RxSubscriber.complete(~exn?);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, subscriber) =>
    subscriber
    |> RxSubscriber.decorate5(
         ~onNext=onNext_,
         ~onComplete=onComplete_,
         onNext,
         onComplete,
         ctx0,
         ctx1,
         ctx2,
       );
};
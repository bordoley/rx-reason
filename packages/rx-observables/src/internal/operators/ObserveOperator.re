let create = (~onNext, ~onComplete) => {
  let onNext_ =
    (. onNext, _, subscriber, next) => {
      onNext(. next);
      subscriber |> RxSubscriber.next(next);
    };
  let onComplete_ =
    (. _, onComplete, subscriber, exn) => {
      onComplete(. exn);
      subscriber |> RxSubscriber.complete(~exn?);
    };

  RxSubscriber.decorate2(
    ~onNext=onNext_,
    ~onComplete=onComplete_,
    onNext,
    onComplete,
  );
};

let create1 = (~onNext, ~onComplete, ctx0) => {
  let onNext_ =
    (. onNext, _, ctx0, subscriber, next) => {
      onNext(ctx0, next);
      subscriber |> RxSubscriber.next(next);
    };
  let onComplete_ =
    (. _, onComplete, ctx0, subscriber, exn) => {
      onComplete(ctx0, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    };
  RxSubscriber.decorate3(
    ~onNext=onNext_,
    ~onComplete=onComplete_,
    onNext,
    onComplete,
    ctx0,
  );
};

let create2 = (~onNext, ~onComplete, ctx0, ctx1) => {
  let onNext_ =
    (. onNext, _, ctx0, ctx1, subscriber, next) => {
      onNext(ctx0, ctx1, next);
      subscriber |> RxSubscriber.next(next);
    };
  let onComplete_ =
    (. _, onComplete, ctx0, ctx1, subscriber, exn) => {
      onComplete(ctx0, ctx1, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    };
  RxSubscriber.decorate4(
    ~onNext=onNext_,
    ~onComplete=onComplete_,
    onNext,
    onComplete,
    ctx0,
    ctx1,
  );
};

let create3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2) => {
  let onNext_ =
    (. onNext, _, ctx0, ctx1, ctx2, subscriber, next) => {
      onNext(ctx0, ctx1, ctx2, next);
      subscriber |> RxSubscriber.next(next);
    };
  let onComplete_ =
    (. _, onComplete, ctx0, ctx1, ctx2, subscriber, exn) => {
      onComplete(ctx0, ctx1, ctx2, exn);
      subscriber |> RxSubscriber.complete(~exn?);
    };

  RxSubscriber.decorate5(
    ~onNext=onNext_,
    ~onComplete=onComplete_,
    onNext,
    onComplete,
    ctx0,
    ctx1,
    ctx2,
  );
};
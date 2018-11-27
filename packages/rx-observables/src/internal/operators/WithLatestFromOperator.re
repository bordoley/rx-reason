type context('a, 'b, 'c) = {
  otherLatest: RxMutableOption.t('b),
  mutable otherSubscription: RxDisposable.t,
  selector: ('a, 'b) => 'c,
  mutable self: RxSubscriber.t('a),
};

let onNext = {
  let impl = ({otherLatest, selector, self}, delegate, next) =>
    if (RxMutableOption.isNotEmpty(otherLatest)) {
      let latest = otherLatest |> RxMutableOption.get;
      let nextWithLatest =
        try (selector(next, latest)) {
        | exn =>
          self |> RxSubscriber.complete(~exn);
          RxFunctions.returnUnit();
        };
      delegate |> RxSubscriber.next(nextWithLatest);
    };

  (ctx, delegate, next) =>
    RxFunctions.earlyReturnsUnit3(impl, ctx, delegate, next);
};

let onComplete = ({otherSubscription}, delegate, exn) => {
  delegate |> RxSubscriber.complete(~exn?);
  otherSubscription |> RxDisposable.dispose;
};

let otherOnNext = (_, otherLatest, next) =>
  otherLatest |> RxMutableOption.set(next);

let otherOnComplete = (self, _, exn) =>
  switch (exn) {
  | Some(_) as exn => self |> RxSubscriber.complete(~exn?)
  | _ => ()
  };

let operator = (~selector, other, subscriber) => {
  let otherLatest = RxMutableOption.create();

  let context = {
    otherLatest,
    otherSubscription: RxDisposable.disposed,
    selector,
    self: RxSubscriber.disposed,
  };

  let self =
    subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, context);

  let innerSubscriber =
    RxSubscriber.create2(
      ~onNext=otherOnNext,
      ~onComplete=otherOnComplete,
      self,
      otherLatest,
    );

  other |> RxObservable.subscribeWith(innerSubscriber);

  context.otherSubscription = innerSubscriber |> RxSubscriber.asDisposable;

  context.self =
    self
    |> RxSubscriber.addTeardown1(
         RxDisposable.dispose,
         context.otherSubscription,
       );
  self;
};
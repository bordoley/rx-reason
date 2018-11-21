type context('a, 'b, 'c) = {
  otherLatest: MutableOption.t('b),
  mutable otherSubscription: Disposable.t,
  selector: ('a, 'b) => 'c,
  mutable self: Subscriber.t('a),
};

let onNext = {
  let impl = ({otherLatest, selector, self}, delegate, next) =>
    if (MutableOption.isNotEmpty(otherLatest)) {
      let latest = otherLatest |> MutableOption.get;
      let nextWithLatest =
        try (selector(next, latest)) {
        | exn =>
          self |> Subscriber.complete(~exn);
          Functions.returnUnit();
        };
      delegate |> Subscriber.next(nextWithLatest);
    };

  (ctx, delegate, next) =>
    Functions.earlyReturnsUnit3(impl, ctx, delegate, next);
};

let onComplete = ({otherSubscription}, delegate, exn) => {
  delegate |> Subscriber.complete(~exn?);
  otherSubscription |> Disposable.dispose;
};

let otherOnNext = (_, otherLatest, next) =>
  otherLatest |> MutableOption.set(next);

let otherOnComplete = (self, _, exn) =>
  switch (exn) {
  | Some(_) as exn => self |> Subscriber.complete(~exn?)
  | _ => ()
  };

let operator = (~selector, other, subscriber) => {
  let otherLatest = MutableOption.create();
  
  let context = {
    otherLatest,
    otherSubscription: Disposable.disposed,
    selector,
    self: Subscriber.disposed,
  };

  let self =
    subscriber |> Subscriber.decorate1(~onNext, ~onComplete, context);

  let innerSubscriber =
    Subscriber.createAutoDisposing2(
      ~onNext=otherOnNext,
      ~onComplete=otherOnComplete,
      self,
      otherLatest,
    );

  other |> Observable.subscribeWith(innerSubscriber);

  context.otherSubscription = innerSubscriber |> Subscriber.asDisposable;

  context.self =
    self
    |> Subscriber.addTeardown1(Disposable.dispose, context.otherSubscription);
  self;
};
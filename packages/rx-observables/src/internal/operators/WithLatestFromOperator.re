type context('a, 'b, 'c) = {
  otherLatest: RxMutableOption.t('b),
  mutable otherSubscription: RxDisposable.t,
  selector: ('a, 'b) => 'c,
};

let onNext = ({otherLatest, selector}, delegate, next) =>
  if (RxMutableOption.isNotEmpty(otherLatest)) {
    let latest = otherLatest |> RxMutableOption.get;
    let nextWithLatest = selector(next, latest);
    delegate |> RxSubscriber.next(nextWithLatest);
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

let create = (~selector, other, subscriber) => {
  let otherLatest = RxMutableOption.create();

  let context = {
    otherLatest,
    otherSubscription: RxDisposable.disposed,
    selector,
  };

  let delegateSubscriber =
    subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, context);

  context.otherSubscription =
    other
    |> RxObservable.lift(
         ObserveOperator.create2(
           ~onNext=otherOnNext,
           ~onComplete=otherOnComplete,
           delegateSubscriber,
           otherLatest,
         ),
       )
    |> RxObservable.connect;

  delegateSubscriber |> RxSubscriber.addDisposable(context.otherSubscription);
};
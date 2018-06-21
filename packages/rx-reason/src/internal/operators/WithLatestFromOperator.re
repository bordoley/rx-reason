type context('a, 'b, 'c) = {
  otherLatest: MutableOption.t('b),
  mutable otherSubscription: Disposable.t,
  selector: ('a, 'b) => 'c,
  mutable self: Subscriber.t('a),
};

let operator = {
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

  (~selector, other, subscriber) => {
    let otherLatest = MutableOption.create();
    let context = {
      otherLatest,
      otherSubscription: Disposable.disposed,
      selector,
      self: Subscriber.disposed,
    };

    let self =
      subscriber |> Subscriber.delegate(~onNext, ~onComplete, context);

    context.otherSubscription =
      ObservableSource.subscribeWith(
        ~onNext=next => otherLatest |> MutableOption.set(next),
        ~onComplete=
          exn =>
            switch (exn) {
            | Some(_) => self |> Subscriber.complete(~exn?)
            | _ => ()
            },
        other,
      )
      |> CompositeDisposable.asDisposable;

    context.self = self |> Subscriber.addDisposable(context.otherSubscription);
    self 
  };
};

let lift = (~selector, other, source) =>
  source |> ObservableSource.lift(operator(~selector, other));
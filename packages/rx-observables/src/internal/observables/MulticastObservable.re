type multicastState('a) = {
  createSubject: unit => RxSubject.t('a),
  mutable refCount: int,
  source: RxObservable.t('a),
  mutable subject: RxSubject.t('a),
  subscription: RxSerialDisposable.t,
};

let forwardOnComplete = (subject, exn) => subject |> RxSubject.complete(~exn?);

let forwardOnNext = (subject, v) => subject |> RxSubject.next(v);

let create = {
  let teardown = state => {
    state.refCount = state.refCount - 1;

    if (state.refCount === 0) {
      state.subscription
      |> RxSerialDisposable.setInnerDisposable(RxDisposable.disposed);
      state.subject |> RxSubject.dispose;
      state.subject = RxSubject.disposed;
    };
  };

  let source = (state, subscriber) => {
    if (state.refCount === 0) {
      state.subject = state.createSubject();
    };

    state.refCount = state.refCount + 1;

    let innerSubscription =
      state.subject
      |> RxSubject.asObservable
      |> RxObservable.observe1(
           ~onNext=SubscriberForward.onNext,
           ~onComplete=SubscriberForward.onComplete,
           subscriber,
         )
      |> RxObservable.subscribe;

    subscriber
    |> RxSubscriber.addTeardown1(teardown, state)
    |> RxSubscriber.addTeardown1(RxDisposable.dispose, innerSubscription)
    |> ignore;

    if (state.refCount === 1) {
      let subscriber =
        state.source
        |> RxObservable.observe1(
             ~onNext=forwardOnNext,
             ~onComplete=forwardOnComplete,
             state.subject,
           )
        |> RxObservable.subscribe;

      state.subscription |> RxSerialDisposable.setInnerDisposable(subscriber);
    };
  };

  (createSubject, observable) => {
    let state = {
      createSubject,
      refCount: 0,
      source: observable,
      subscription: RxSerialDisposable.create(),
      subject: RxSubject.disposed,
    };

    RxObservable.create1(source, state);
  };
};
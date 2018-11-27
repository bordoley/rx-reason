type multicastState('a) = {
  createSubject: unit => RxSubject.t('a),
  mutable refCount: int,
  source: RxObservable.t('a),
  mutable subject: RxSubject.t('a),
  subscription: RxSerialDisposable.t,
};

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
    subscriber |> RxSubscriber.addTeardown1(teardown, state) |> ignore;
    state.subject |> RxSubject.subscribeWith(subscriber);

    if (state.refCount === 1) {
      let subscriber =
        RxSubscriber.create1(
          ~onNext=RxSubject.forwardOnNext,
          ~onComplete=RxSubject.forwardOnComplete,
          state.subject,
        );

      state.source |> RxObservable.subscribeWith(subscriber);

      state.subscription
      |> RxSerialDisposable.setInnerDisposable(
           subscriber |> RxSubscriber.asDisposable,
         );
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
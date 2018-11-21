type multicastState('a) = {
  createSubject: unit => Subject.t('a),
  mutable refCount: int,
  source: Observable.t('a),
  mutable subject: Subject.t('a),
  subscription: SerialDisposable.t,
};

let create = {
  let teardown = state => {
    state.refCount = state.refCount - 1;

    if (state.refCount === 0) {
      state.subscription
      |> SerialDisposable.setInnerDisposable(Disposable.disposed);
      state.subject |> Subject.dispose;
      state.subject = Subject.disposed;
    };
  };

  let source = (state, subscriber) => {
    if (state.refCount === 0) {
      state.subject = state.createSubject();
    };

    state.refCount = state.refCount + 1;
    subscriber |> Subscriber.addTeardown1(teardown, state) |> ignore;
    state.subject |> Subject.subscribeWith(subscriber);

    if (state.refCount === 1) {
      let subscriber =
        Subscriber.create1(
          ~onNext=Subject.forwardOnNext,
          ~onComplete=Subject.forwardOnComplete,
          state.subject,
        );

      state.source |> Observable.subscribeWith(subscriber);

      state.subscription
      |> SerialDisposable.setInnerDisposable(
           subscriber |> Subscriber.asDisposable,
         );
    };
  };

  (createSubject, observable) => {
    let state = {
      createSubject,
      refCount: 0,
      source: observable,
      subscription: SerialDisposable.create(),
      subject: Subject.disposed,
    };

    Observable.create1(source, state);
  };
};

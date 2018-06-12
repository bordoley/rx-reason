type t('a) = Observable.t('a);

let subscribe = Observable.subscribe;

let subscribeObserver = Observable.subscribeObserver;

let subscribeWithCallbacks = Observable.subscribeWithCallbacks;

let toObservable = Functions.identity;

let shareInternal = (~createSubject, source) => {
  let subject = ref(Subject.disposed);
  let sourceSubscription = AssignableDisposable.create();
  let refCount = ref(0);

  let reset = () => {
    sourceSubscription |> AssignableDisposable.set(Disposable.disposed);
    subject^ |> Subject.dispose;
    subject := Subject.disposed;
    refCount := 0;
  };

  Observable.create((~onNext, ~onComplete) => {
    /* FIXME: Should probably add some locking here */
    if (refCount^ === 0) {
      subject := createSubject();
    };
    let subject = subject^;

    let subscription =
      subject
      |> Subject.toObservable
      |> Observable.subscribeWithCallbacks(~onNext, ~onComplete);

    if (refCount^ === 0) {
      let observer = subject |> Subject.toObserver;
      sourceSubscription
      |> AssignableDisposable.set(
           Observable.subscribeObserver(observer, source),
         );
    };

    if (subscription |> Disposable.isDisposed) {
      Disposable.disposed;
    } else {
      refCount := refCount^ + 1;
      Disposable.create(() => {
        refCount := refCount^ > 0 ? refCount^ - 1 : 0;
        if (refCount^ === 0) {
          reset();
        };
        subscription |> Disposable.dispose;
      });
    };
  });
};

let share = obs => shareInternal(~createSubject=Subject.create, obs);

let shareWithReplayBuffer = (count, obs) =>
  shareInternal(
    ~createSubject=() => Subject.createWithReplayBuffer(count),
    obs,
  );
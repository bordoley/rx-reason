type t('a) = Observable.t('a);

let asObservable = Functions.identity;

let subscribe = Observable.subscribe;

let subscribeWith = Observable.subscribeWith;

let publish = Observable.publish;

let publishTo = Observable.publishTo;

let shareInternal = (~createSubject, source) => {
  let subject = ref(Subject.disposed);
  let sourceSubscription = SerialDisposable.create();
  let refCount = ref(0);

  Observable.create((~onNext, ~onComplete) => {
    /* FIXME: Should probably add some locking here */
    if (refCount^ === 0) {
      subject := createSubject();
    };
    let currentSubject = subject^;

    let subscription =
    currentSubject
      |> Subject.subscribeWith(~onNext, ~onComplete);

    if (refCount^ === 0) {
      let observer = currentSubject |> Subject.asObserver;
      sourceSubscription
      |> SerialDisposable.set(
           Observable.subscribeWith(
             ~onNext=Observer.forwardOnNext(observer), 
             ~onComplete=Observer.forwardOnComplete(observer),
             source
            ),
         );
    };

    if (subscription |> Disposable.isDisposed) {
      Disposable.disposed;
    } else {
      incr(refCount);
      Disposable.create(() => {
        decr(refCount);
        if (refCount^ === 0) {
          sourceSubscription |> SerialDisposable.set(Disposable.disposed);
          currentSubject |> Subject.dispose;
          subject := Subject.disposed;
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
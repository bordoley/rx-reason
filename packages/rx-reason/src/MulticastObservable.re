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

  Observable.create(observer => {
    /* FIXME: Should probably add some locking here */
    if (refCount^ === 0) {
      subject := createSubject();
    };
    let currentSubject = subject^;

    let subscription =
      currentSubject
      |> Subject.subscribeWith(
           ~onNext=Observer.forwardOnNext(observer),
           ~onComplete=Observer.forwardOnComplete(observer),
         );

    if (refCount^ === 0) {
      sourceSubscription
      |> SerialDisposable.set(
           Observable.subscribeWith(
             ~onNext=next => currentSubject |> Subject.next(next),
             ~onComplete=exn => currentSubject |> Subject.complete(~exn?),
             source,
           )
           |> CompositeDisposable.asDisposable,
         );
    };

    let teardown = () => {
      decr(refCount);
      if (refCount^ === 0) {
        sourceSubscription |> SerialDisposable.set(Disposable.disposed);
        currentSubject |> Subject.dispose;
        subject := Subject.disposed;
      };
      subscription |> CompositeDisposable.dispose;
    };

    if (!CompositeDisposable.isDisposed(subscription)) {
      incr(refCount);
      observer |> Observer.addTeardown(teardown) |> ignore;
    };
  });
};

let share = obs => shareInternal(~createSubject=Subject.create, obs);

let shareWithReplayBuffer = (count, obs) =>
  shareInternal(
    ~createSubject=() => Subject.createWithReplayBuffer(count),
    obs,
  );
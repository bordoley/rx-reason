type t('a) = Observable.t('a);

let asObservable = Functions.identity;

let subscribe = Observable.subscribe;

let subscribeWith = Observable.subscribeWith;

let publish = Observable.publish;

let publishTo = Observable.publishTo;

let shareInternal = {
  let teardown =
      (refCount, sourceSubscription, currentSubject, subject, subscription) => {
    decr(refCount);
    if (refCount^ === 0) {
      sourceSubscription |> SerialDisposable.set(Disposable.disposed);
      currentSubject |> Subject.dispose;
      subject := Subject.disposed;
    };
    subscription |> CompositeDisposable.dispose;
  };

  (~createSubject, source) => {
    let subject = ref(Subject.disposed);
    let sourceSubscription = SerialDisposable.create();
    let refCount = ref(0);

    Observable.create(subscriber => {
      /* FIXME: Should probably add some locking here */
      if (refCount^ === 0) {
        subject := createSubject();
      };
      let currentSubject = subject^;

      let subscription =
        currentSubject
        |> Subject.subscribeWith(
             ~onNext=next => subscriber |> Subscriber.next(next),
             ~onComplete=exn => subscriber |> Subscriber.complete(~exn?),
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

      if (! CompositeDisposable.isDisposed(subscription)) {
        incr(refCount);
        subscriber
        |> Subscriber.addTeardown5(
             teardown,
             refCount,
             sourceSubscription,
             currentSubject,
             subject,
             subscription,
           )
        |> ignore;
      };
    });
  };
};

let share = obs => shareInternal(~createSubject=Subject.create, obs);

let shareWithReplayBuffer = (count, obs) =>
  shareInternal(
    ~createSubject=() => Subject.createWithReplayBuffer(count),
    obs,
  );
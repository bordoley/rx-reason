type t('a) = Observable.t('a);

let asObservable = Functions.identity;
let subscribeWith = Observable.subscribeWith;
let subscribe = Observable.subscribe;

let shareInternal = {
  let teardown =
      (refCount, sourceSubscription, currentSubject, subject, subscription) => {
    decr(refCount);
    if (refCount^ === 0) {
      sourceSubscription
      |> SerialDisposable.setInnerDisposable(Disposable.disposed);
      currentSubject |> Subject.dispose;
      subject := Subject.disposed;
    };
    subscription |> Disposable.dispose;
  };

  let source =
      (
        refCount,
        sourceSubscription,
        subject,
        createSubject,
        source,
        subscriber,
      ) => {
    /* FIXME: Should probably add some locking here */
    if (refCount^ === 0) {
      subject := createSubject();
    };
    let currentSubject = subject^;

    let subscription =
      Subscriber.createAutoDisposing1(
        ~onNext=Subscriber.forwardOnNext,
        ~onComplete=Subscriber.forwardOnComplete,
        subscriber,
      );
    currentSubject |> Subject.subscribeWith(subscription);

    if (refCount^ === 0) {
      let newSubscription =
        Subscriber.createAutoDisposing1(
          ~onNext=Subject.forwardOnNext,
          ~onComplete=Subject.forwardOnComplete,
          currentSubject,
        );

      sourceSubscription
      |> SerialDisposable.setInnerDisposable(
           newSubscription |> Subscriber.asDisposable,
         );
      source |> Observable.subscribeWith(newSubscription);
    };

    if (! Subscriber.isDisposed(subscription)) {
      incr(refCount);
      subscriber
      |> Subscriber.addTeardown5(
           teardown,
           refCount,
           sourceSubscription,
           currentSubject,
           subject,
           subscription |> Subscriber.asDisposable,
         )
      |> ignore;
    };
  };

  (~createSubject, observable) => {
    let subject = ref(Subject.disposed);
    let sourceSubscription = SerialDisposable.create();
    let refCount = ref(0);

    Observable.create5(
      source,
      refCount,
      sourceSubscription,
      subject,
      createSubject,
      observable,
    );
  };
};

let share = obs => shareInternal(~createSubject=Subject.create, obs);

let shareWithReplayBuffer = count => {
  let createSubject = () => Subject.createWithReplayBuffer(count);
  obs => shareInternal(~createSubject, obs);
};
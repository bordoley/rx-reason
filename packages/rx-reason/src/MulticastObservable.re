type t('a) = Observable.t('a);

let asObservable = Functions.identity;
let subscribeWith = Observable.subscribeWith;
let subscribeWith1 = Observable.subscribeWith1;
let subscribeWith2 = Observable.subscribeWith2;
let subscribeWith3 = Observable.subscribeWith3;
let subscribeWith4 = Observable.subscribeWith4;
let subscribeWith5 = Observable.subscribeWith5;
let subscribeWith6 = Observable.subscribeWith6;
let subscribeWith7 = Observable.subscribeWith7;
let subscribe = Observable.subscribe;
let subscribe1 = Observable.subscribe1;
let subscribe2 = Observable.subscribe2;
let subscribe3 = Observable.subscribe3;
let subscribe4 = Observable.subscribe4;
let subscribe5 = Observable.subscribe5;
let subscribe6 = Observable.subscribe6;
let subscribe7 = Observable.subscribe7;

let publish = Observable.publish;
let publish1 = Observable.publish1;
let publishTo = Observable.publishTo;
let publishTo1 = Observable.publishTo1;

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
        |> Subject.subscribeWith1(
             ~onNext=Subscriber.delegateOnNext,
             ~onComplete=Subscriber.delegateOnComplete,
             subscriber,
           );

      if (refCount^ === 0) {
        sourceSubscription
        |> SerialDisposable.set(
             Observable.subscribeWith1(
               ~onNext=Subject.delegateOnNext,
               ~onComplete=Subject.delegateOnComplete,
               currentSubject,
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
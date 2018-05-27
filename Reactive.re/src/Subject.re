type t('a) = {
  observer: Observer.t('a),
  observable: Observable.t('a),
};

let toObserver = ({observer}: t('a)) : Observer.t('a) => observer;

let toObservable = ({observable}: t('a)) : Observable.t('a) => observable;

let createWithCallbacks =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      ~onDispose=Functions.alwaysUnit,
      ~onSubscribe=(~onNext as _, ~onComplete as _) => Disposable.disposed,
      (),
    )
    : t('a) => {
  let subscribers = ref(CopyOnWriteArray.empty());
  let observer =
    Observer.create(
      ~onComplete=
        exn => {
          let currentSubscribers = subscribers^;
          onComplete(exn);
          currentSubscribers
          |> CopyOnWriteArray.forEach(((_, onComplete)) => onComplete(exn));
        },
      ~onNext=
        next => {
          onNext(next);
          let currentSubscribers = subscribers^;
          currentSubscribers
          |> CopyOnWriteArray.forEach(((onNext, _)) => onNext(next));
          ();
        },
      ~onDispose=
        () => {
          onDispose();
          subscribers := CopyOnWriteArray.empty();
        },
    );
  let observable =
    Observable.create((~onNext, ~onComplete) => {
      observer |> Observer.toDisposable |> Disposable.raiseIfDisposed;
      let currentSubscribers = subscribers^;
      let subscriber = (onNext, onComplete);
      subscribers := currentSubscribers |> CopyOnWriteArray.addLast(subscriber);
      let onSubscribeDisposable = onSubscribe(~onNext, ~onComplete);
      Disposable.create(() => {
        Disposable.dispose(onSubscribeDisposable);
        let currentSubscribers = subscribers^;
        subscribers :=
          currentSubscribers
          |> CopyOnWriteArray.findAndRemove(
               Functions.referenceEquality,
               subscriber,
             );
      });
    });
  {observer, observable};
};

let create = () => createWithCallbacks();

/* let createWithReplay */
let share =
    (~createSubject=create, source: Observable.t('a))
    : Observable.t('a) => {
  let subject = MutableOption.create();
  let sourceSubscription = ref(Disposable.disposed);
  let refCount = ref(0);
  let reset = () => {
    sourceSubscription^ |> Disposable.dispose;
    subject |> MutableOption.unset;
    refCount := 0;
  };
  Observable.create((~onNext, ~onComplete) => {
    let currentSubject = {
      if (refCount^ === 0) {
        MutableOption.set(createSubject(), subject);
      };
      subject |> MutableOption.get;
    };
    let subjectObservable = toObservable(currentSubject);
    let subjectObserver = toObserver(currentSubject);
    let observerSubscription =
      subjectObservable |> Observable.subscribe(~onNext, ~onComplete);
    if (refCount^ === 0) {
      sourceSubscription :=
        source
        |> Observable.subscribe(
             ~onNext=next => subjectObserver |> Observer.next(next),
             ~onComplete=
               exn => {
                 subjectObserver |> Observer.complete(exn);
                 reset();
               },
           );
    };
    if (subjectObserver |> Observer.toDisposable |> Disposable.isDisposed) {
      /* The source completed synchronously and reset */
      observerSubscription |> Disposable.dispose;
      Disposable.disposed;
    } else {
      refCount := refCount^ + 1;
      Disposable.create(() => {
        refCount := refCount^ > 0 ? refCount^ - 1 : 0;
        if (refCount^ === 0) {
          reset();
        };
        observerSubscription |> Disposable.dispose;
      });
    };
  });
};
/* shareReplay */
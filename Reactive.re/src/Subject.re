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
      ~onSubscribe=(_: Observer.t('a)) => Disposable.disposed,
      (),
    )
    : t('a) => {
  let subscribers = ref(CopyOnWriteArray.empty());
  let subjectObserver =
    Observer.create(
      ~onComplete=
        exn => {
          let currentSubscribers = subscribers^;
          onComplete(exn);
          currentSubscribers
          |> CopyOnWriteArray.forEach(Observer.complete(~exn));
        },
      ~onNext=
        next => {
          onNext(next);
          let currentSubscribers = subscribers^;
          currentSubscribers |> CopyOnWriteArray.forEach(Observer.next(next));
          ();
        },
      ~onDispose=
        () => {
          onDispose();
          subscribers := CopyOnWriteArray.empty();
        },
      (),
    );
  let observable =
    Observable.create(observer => {
      subjectObserver |> Observer.toDisposable |> Disposable.raiseIfDisposed;
      let currentSubscribers = subscribers^;
      subscribers := currentSubscribers |> CopyOnWriteArray.addLast(observer);
      let onSubscribeDisposable = onSubscribe(observer);
      Disposable.create(() => {
        Disposable.dispose(onSubscribeDisposable);
        let currentSubscribers = subscribers^;
        subscribers :=
          currentSubscribers
          |> CopyOnWriteArray.findAndRemove(
               Functions.referenceEquality,
               observer,
             );
      });
    });
  {observer: subjectObserver, observable};
};

let create = () => createWithCallbacks();

/* let createWithReplay */
let share =
    (~createSubject=create, source: Observable.t('a))
    : Observable.t('a) => {
  let subject = MutableOption.empty();
  let sourceSubscription = ref(Disposable.disposed);
  let refCount = ref(0);
  let reset = () => {
    sourceSubscription^ |> Disposable.dispose;
    subject |> MutableOption.unset;
    refCount := 0;
  };
  Observable.create((observer: Observer.t('a)) => {
    let currentSubject = {
      if (refCount^ === 0) {
        MutableOption.set(createSubject(), subject);
      };
      subject |> MutableOption.firstOrRaise;
    };
    let subjectObservable = toObservable(currentSubject);
    let subjectObserver = toObserver(currentSubject);
    let observerSubscription =
      subjectObservable |> Observable.subscribeObserver(observer);
    if (refCount^ === 0) {
      sourceSubscription :=
        source
        |> Observable.subscribe(
             ~onNext=next => subjectObserver |> Observer.next(next),
             ~onComplete=
               exn => {
                 subjectObserver |> Observer.complete(~exn);
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
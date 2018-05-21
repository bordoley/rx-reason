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
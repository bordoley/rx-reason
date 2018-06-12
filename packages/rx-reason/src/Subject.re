type t('a) = {
  observer: Observer.t('a),
  observable: Observable.t('a),
};

let disposed = {observer: Observer.disposed, observable: Observable.never};

let complete = (exn, {observer}) => observer |> Observer.complete(exn);

let completeWithResult = (exn, {observer}) =>
  observer |> Observer.completeWithResult(exn);

let dispose = ({observer}) => observer |> Observer.dispose;

let isDisposed = ({observer}) => observer |> Observer.isDisposed;

let isStopped = ({observer}) => observer |> Observer.isStopped;

let next = (next, {observer}) => observer |> Observer.next(next);

let raiseIfDisposed = ({observer}) => observer |> Observer.raiseIfDisposed;

let toDisposable = ({observer}) => observer |> Observer.toDisposable;

let toObserver = ({observer}: t('a)) : Observer.t('a) => observer;

let toObservable = ({observable}: t('a)) : Observable.t('a) => observable;

let subscribe = subject => subject |> toObservable |> Observable.subscribe;

let subscribeWithCallbacks = (~onNext, ~onComplete, subject) =>
  subject
  |> toObservable
  |> Observable.subscribeWithCallbacks(~onNext, ~onComplete);

let createWithCallbacks =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      ~onDispose=Functions.alwaysUnit,
      ~onSubscribe=(~onNext as _, ~onComplete as _) => (),
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
      observer |> Observer.raiseIfDisposed;

      let subscriber = (onNext, onComplete);
      subscribers := subscribers^ |> CopyOnWriteArray.addLast(subscriber);

      onSubscribe(~onNext, ~onComplete);

      Disposable.create(() => {
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

let createWithReplayBuffer = (maxBufferCount: int) : t('a) => {
  let buffer = ref(CopyOnWriteArray.empty());
  let completed = ref(false);
  let completedValue = ref(None);

  createWithCallbacks(
    ~onNext=
      next => {
        let currentBuffer = buffer^;
        let nextBuffer =
          if (CopyOnWriteArray.count(currentBuffer) === maxBufferCount) {
            currentBuffer |> CopyOnWriteArray.removeAt(0);
          } else {
            currentBuffer;
          };
        buffer := nextBuffer |> CopyOnWriteArray.addLast(next);
      },
    ~onComplete=
      exn => {
        completed := true;
        completedValue := exn;
      },
    ~onDispose=() => buffer := CopyOnWriteArray.empty(),
    ~onSubscribe=
      (~onNext, ~onComplete) => {
        let currentBuffer = buffer^;
        CopyOnWriteArray.forEach(onNext, currentBuffer);
        if (completed^) {
          onComplete(completedValue^);
        };
      },
    (),
  );
};
type t('a) = {
  isAutoDispose: bool,
  isStopped: ref(bool),
  onComplete: option(exn) => unit,
  onNext: 'a => unit,
  disposable: CompositeDisposable.t,
};

type observer('a) = t('a);

module type S1 = {
  type t('a);

  include CompositeDisposable.S1 with type t('a) := t('a);

  let asObserver: t('a) => observer('a);
  let complete: (~exn: exn=?, t('a)) => unit;
  let completeWithResult: (~exn: exn=?, t('a)) => bool;
  let isStopped: t('a) => bool;
  let next: ('a, t('a)) => unit;
  let notify: (Notification.t('a), t('a)) => unit;
};

let asCompositeDisposable = ({disposable}) => disposable;

let addDisposable = (disposable, observer) => {
  observer
  |> asCompositeDisposable
  |> CompositeDisposable.addDisposable(disposable)
  |> ignore;
  observer;
};

let addTeardown = (teardown, observer) => {
  observer
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown(teardown)
  |> ignore;
  observer;
};

let asDisposable = observer =>
  observer |> asCompositeDisposable |> CompositeDisposable.asDisposable;

let asObserver = Functions.identity;

let createInternal = (~isAutoDispose, ~onNext, ~onComplete, ~disposable) => {
  let isStopped = ref(false);
  disposable
  |> CompositeDisposable.addTeardown(() => Volatile.write(true, isStopped))
  |> ignore;
  {isAutoDispose, isStopped, onComplete, onNext, disposable};
};

let delegate = (~onNext, ~onComplete, observer) => {
  let disposable = observer |> asCompositeDisposable;
  createInternal(~isAutoDispose=false, ~onNext, ~onComplete, ~disposable);
};

let create = (~onNext, ~onComplete) =>
  createInternal(
    ~isAutoDispose=false,
    ~onNext,
    ~onComplete,
    ~disposable=CompositeDisposable.create(),
  );

let createAutoDisposing = (~onNext, ~onComplete) =>
  createInternal(
    ~isAutoDispose=true,
    ~onNext,
    ~onComplete,
    ~disposable=CompositeDisposable.create(),
  );

let autoDispose = ({isAutoDispose, disposable}) =>
  if (isAutoDispose) {
    disposable |> CompositeDisposable.dispose;
  };

let completeWithResult = (~exn=?, {isStopped, onComplete} as observer) : bool => {
  let shouldComplete = ! Interlocked.exchange(true, isStopped);
  if (shouldComplete) {
    try (onComplete(exn)) {
    | exn =>
      autoDispose(observer);
      raise(exn);
    };
  };
  autoDispose(observer);
  shouldComplete;
};

let complete = (~exn=?, observer: t('a)) : unit =>
  observer |> completeWithResult(~exn?) |> ignore;

let dispose = ({disposable}) => disposable |> CompositeDisposable.dispose;

let disposedIsStopped = ref(true);
let disposed = {
  isAutoDispose: false,
  isStopped: disposedIsStopped,
  onComplete: Functions.alwaysUnit,
  onNext: Functions.alwaysUnit,
  disposable: CompositeDisposable.disposed,
};

let isDisposed = ({disposable}) =>
  disposable |> CompositeDisposable.isDisposed;

let isStopped = ({isStopped}) => Volatile.read(isStopped);

let next = (next: 'a, {onNext, isStopped} as observer) : unit => {
  let isStopped = Volatile.read(isStopped);
  if (! isStopped) {
    try (onNext(next)) {
    | exn =>
      autoDispose(observer);
      raise(exn);
    };
  };
};

let notify = (notif, observer) =>
  switch (notif) {
  | Notification.Next(v) => observer |> next(v)
  | Notification.Complete => observer |> complete
  | Notification.CompleteWithException(exn) => observer |> complete(~exn)
  };

let raiseIfDisposed = ({disposable}) =>
  disposable |> CompositeDisposable.raiseIfDisposed;

let forwardOnComplete = (observer, exn) => observer |> complete(~exn?);

let forwardOnNext = (observer, v) => observer |> next(v);

let forwardOnDispose = (observer, ()) => observer |> dispose;
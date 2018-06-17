type t('a) = {
  isAutoDispose: bool,
  isStopped: ref(bool),
  onComplete: option(exn) => unit,
  onNext: 'a => unit,
  disposable: Disposable.t,
};

type observer('a) = t('a);

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let asObserver: t('a) => observer('a);
  let complete: (~exn: exn=?, t('a)) => unit;
  let completeWithResult: (~exn: exn=?, t('a)) => bool;
  let isStopped: t('a) => bool;
  let next: ('a, t('a)) => unit;
  let notify: (Notification.t('a), t('a)) => unit;
};

let asDisposable = ({disposable}: t('a)) : Disposable.t => disposable;

let asObserver = Functions.identity;

let createInternal = (~isAutoDispose, ~onNext, ~onComplete, ~onDispose) => {
  let isStopped = ref(false);
  {
    isAutoDispose,
    isStopped,
    onComplete,
    onNext,
    disposable:
      Disposable.create(() => {
        Volatile.write(true, isStopped);
        onDispose();
      }),
  };
};

let create = (~onNext, ~onComplete, ~onDispose) =>
  createInternal(~isAutoDispose=false, ~onNext, ~onComplete, ~onDispose);

let createAutoDisposing = (~onNext, ~onComplete, ~onDispose) =>
  createInternal(~isAutoDispose=true, ~onNext, ~onComplete, ~onDispose);

let autoDispose = ({isAutoDispose, disposable}) =>
  if (isAutoDispose) {
    disposable |> Disposable.dispose;
  };

let completeWithResult =
    (~exn=?, {isStopped, onComplete} as observer)
    : bool => {
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

let dispose = ({disposable}) => disposable |> Disposable.dispose;

let disposedIsStopped = ref(true);
let disposed = {
  isAutoDispose: false,
  isStopped: disposedIsStopped,
  onComplete: Functions.alwaysUnit,
  onNext: Functions.alwaysUnit,
  disposable: Disposable.disposed,
};

let isDisposed = ({disposable}) => disposable |> Disposable.isDisposed;

let isStopped = ({isStopped}) => Volatile.read(isStopped);

let next =
    (next: 'a, {onNext, isStopped} as observer)
    : unit => {
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
  disposable |> Disposable.raiseIfDisposed;

let forwardOnComplete = (observer, exn) => observer |> complete(~exn?);

let forwardOnNext = (observer, v) => observer |> next(v);

let forwardOnDispose = (observer, ()) => observer |> dispose;
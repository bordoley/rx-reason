type t('a) = {
  isStopped: ref(bool),
  onComplete: option(exn) => unit,
  onNext: 'a => unit,
  disposable: Disposable.t,
};

type observer('a) = t('a);

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let complete: (~exn: exn=?, t('a)) => unit;
  let completeWithResult: (~exn: exn=?, t('a)) => bool;
  let isStopped: t('a) => bool;
  let next: ('a, t('a)) => unit;
  let notify: (Notification.t('a), t('a)) => unit;
  let toObserver: t('a) => observer('a);
};

let create = (~onNext, ~onComplete, ~onDispose) : t('a) => {
  let isStopped = ref(false);
  {
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

let createAutoDisposing = (~onNext, ~onComplete, ~onDispose) : t('a) => {
  let isStopped = ref(false);
  let disposable =
    Disposable.create(() => {
      Volatile.write(true, isStopped);
      onDispose();
    });

  {
    isStopped,
    onComplete: exn => {
      try (onComplete(exn)) {
      | exn =>
        disposable |> Disposable.dispose;
        raise(exn);
      };
      disposable |> Disposable.dispose;
    },
    onNext: next =>
      try (onNext(next)) {
      | exn =>
        disposable |> Disposable.dispose;
        raise(exn);
      },
    disposable,
  };
};

let completeWithResult = (~exn=?, {isStopped, onComplete}: t('a)) : bool => {
  let shouldComplete = ! Interlocked.exchange(true, isStopped);
  if (shouldComplete) {
    onComplete(exn);
  };
  shouldComplete;
};

let complete = (~exn=?, observer: t('a)) : unit =>
  observer |> completeWithResult(~exn?) |> ignore;

let dispose = ({disposable}) => disposable |> Disposable.dispose;

let disposedIsStopped = ref(true);
let disposed = {
  isStopped: disposedIsStopped,
  onComplete: Functions.alwaysUnit,
  onNext: Functions.alwaysUnit,
  disposable: Disposable.disposed,
};

let isDisposed = ({disposable}) => disposable |> Disposable.isDisposed;

let isStopped = ({isStopped}) => Volatile.read(isStopped);

let next = (next: 'a, {onNext, isStopped}: t('a)) : unit => {
  let isStopped = Volatile.read(isStopped);
  if (! isStopped) {
    onNext(next);
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

let toDisposable = ({disposable}: t('a)) : Disposable.t => disposable;

let toObserver = Functions.identity;

let forwardOnComplete = (observer, exn) => observer |> complete(~exn?);

let forwardOnNext = (observer, v) => observer |> next(v);

let forwardOnDispose = (observer, ()) => observer |> dispose;
type t('a) = {
  isStopped: ref(bool),
  onComplete: option(exn) => unit,
  onNext: 'a => unit,
  disposable: Disposable.t,
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

let toDisposable = ({disposable}: t('a)) : Disposable.t => disposable;

let completeWithResult =
    (~exn=None, {isStopped, onComplete, disposable}: t('a))
    : bool => {
  let shouldComplete = ! Interlocked.exchange(true, isStopped);
  if (shouldComplete) {
    try (onComplete(exn)) {
    | exn =>
      disposable |> Disposable.dispose;
      raise(exn);
    };
  };
  disposable |> Disposable.dispose;
  shouldComplete;
};

let complete = (~exn=None, observer: t('a)) : unit =>
  observer |> completeWithResult(~exn) |> ignore;

let next = (next: 'a, {onNext, isStopped, disposable}: t('a)) : unit => {
  let isStopped = Volatile.read(isStopped);
  if (! isStopped) {
    try (onNext(next)) {
    | exn =>
      disposable |> Disposable.dispose;
      raise(exn);
    };
  };
};
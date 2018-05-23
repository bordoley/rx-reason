type t('a) = {
  isStopped: ref(bool),
  onComplete: option(exn) => unit,
  onNext: 'a => unit,
  disposable: Disposable.t,
};

let createWithCallbacks = (~onComplete, ~onNext, ~onDispose) : t('a) => {
  let isStopped = ref(false);
  {
    isStopped,
    onComplete,
    onNext,
    disposable: Disposable.create(() => {
      Volatile.write(true, isStopped);
      onDispose();
    }),
  };
};

let create =
    (
      ~onComplete=Functions.alwaysUnit,
      ~onNext=Functions.alwaysUnit,
      ~onDispose=Functions.alwaysUnit,
      (),
    )
    : t('a) =>
  createWithCallbacks(~onComplete, ~onNext, ~onDispose);

let completeWithResult = (~exn=None, {isStopped, onComplete, disposable}: t('a)) : bool => {
  let shouldComplete = !Interlocked.exchange(true, isStopped);
  if (shouldComplete) {
    onComplete(exn);
  };
  disposable|> Disposable.dispose;
  shouldComplete;
};

let next = (next: 'a, {onNext, isStopped}: t('a)) : unit => {
  let isStopped = Volatile.read(isStopped);
  if (! isStopped) {
    onNext(next);
  };
};

let complete = (~exn=None, observer: t('a)) : unit =>
  observer |> completeWithResult(~exn) |> ignore;

let toDisposable = ({disposable}: t('a)) : Disposable.t => disposable;
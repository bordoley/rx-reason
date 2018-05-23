type t('a) = {
  onComplete: option(exn) => unit,
  onNext: 'a => unit,
  state: Disposable.t,
};

let create =
    (
      ~onComplete=Functions.alwaysUnit,
      ~onNext=Functions.alwaysUnit,
      ~onDispose=Functions.alwaysUnit,
      (),
    )
    : t('a) => {
  onComplete,
  onNext,
  state: Disposable.create(onDispose),
};

let completeWithResult = (~exn=None, {onComplete, state}: t('a)) : bool => {
  let shouldComplete = Disposable.disposeWithResult(state);
  if (shouldComplete) {
    onComplete(exn);
  };
  shouldComplete;
};

let next = (next: 'a, {onNext, state}: t('a)) : unit => {
  let isDisposed = Disposable.isDisposed(state);
  if (! isDisposed) {
    onNext(next);
  };
};

let complete = (~exn=None, observer: t('a)) : unit =>
  observer |> completeWithResult(~exn) |> ignore;

let toDisposable = ({state}: t('a)) : Disposable.t => state;
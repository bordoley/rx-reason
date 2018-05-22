open Functions.Operators;

type t = {
  mutable dispose: unit => unit,
  isDisposed: ref(bool),
};

exception DisposedException;

let create = (dispose: unit => unit) : t => {
  dispose,
  isDisposed: ref(false),
};

let disposeWithResult = ({dispose, isDisposed} as disposable: t) : bool => {
  let shouldDispose = ! Concurrency.interlockedExchange(true, isDisposed);
  if (shouldDispose) {
    dispose();
    disposable.dispose = Functions.alwaysUnit;
  };
  shouldDispose;
};

let dispose = (disposable: t) : unit =>
  disposable |> disposeWithResult |> ignore;

let isDisposed = ({isDisposed}: t) : bool =>
  Concurrency.volatileRead(isDisposed);

let empty = () => create(Functions.alwaysUnit);

let disposed: t = {
  let retval = empty();
  retval |> dispose |> ignore;
  retval;
};

let compose = (disposables: list(t)) : t => {
  let dispose = () => disposables |> List.iter(dispose >> ignore);
  create(dispose);
};

let raiseIfDisposed = (disposable: t) : unit =>
  if (isDisposed(disposable)) {
    raise(DisposedException);
  };
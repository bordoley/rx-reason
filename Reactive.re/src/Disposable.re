open Functions.Operators;

type t = {
  mutable onDispose: unit => unit,
  isDisposed: ref(bool),
};

exception DisposedException;

let create = (onDispose: unit => unit) : t => {
  onDispose,
  isDisposed: ref(false),
};

let disposeWithResult = ({onDispose, isDisposed} as disposable: t) : bool => {
  let shouldDispose = ! Interlocked.exchange(true, isDisposed);
  if (shouldDispose) {
    onDispose();
    disposable.onDispose = Functions.alwaysUnit;
  };
  shouldDispose;
};

let dispose = (disposable: t) : unit =>
  disposable |> disposeWithResult |> ignore;

let isDisposed = ({isDisposed}: t) : bool =>
  Volatile.read(isDisposed);

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
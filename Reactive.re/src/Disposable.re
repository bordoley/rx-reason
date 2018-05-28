open Functions.Operators;

type t = {
  onDispose: ref(unit => unit),
  isDisposed: ref(bool),
};

exception DisposedException;

let create = (onDispose: unit => unit) : t => {
  onDispose: ref(onDispose),
  isDisposed: ref(false),
};


let disposeWithResult = ({onDispose, isDisposed}: t) : bool => {
  let shouldDispose = ! Interlocked.exchange(true, isDisposed);
  if (shouldDispose) {
    let onDispose = Interlocked.exchange(Functions.alwaysUnit, onDispose);
    onDispose();
  };
  shouldDispose;
};

let dispose = (disposable: t) : unit =>
  disposable |> disposeWithResult |> ignore;

let compose = (disposables: list(t)) : t => {
  let dispose = () => disposables |> List.iter(dispose >> ignore);
  create(dispose);
};

let empty = () => create(Functions.alwaysUnit);

let disposed: t = {
  let retval = empty();
  retval |> dispose |> ignore;
  retval;
};

let isDisposed = ({isDisposed}: t) : bool =>
  Volatile.read(isDisposed);

let raiseIfDisposed = (disposable: t) : unit =>
  if (isDisposed(disposable)) {
    raise(DisposedException);
  };
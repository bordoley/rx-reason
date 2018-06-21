type t = {
  disposableRef: ref(Disposable.t),
  disposable: Disposable.t,
};

let asDisposable = ({disposable}) => disposable;

let create = {
  let teardown = disposableRef =>
    Interlocked.exchange(Disposable.disposed, disposableRef)
    |> Disposable.dispose;

  () => {
    let disposableRef = ref(Disposable.disposed);
    let disposable = Disposable.create1(teardown, disposableRef);
    {disposableRef, disposable};
  };
};

let dispose = ({disposable}) => disposable |> Disposable.dispose;

let isDisposed = ({disposable}) => disposable |> Disposable.isDisposed;

let raiseIfDisposed = ({disposable}) =>
  disposable |> Disposable.raiseIfDisposed;

let get = ({disposableRef}) => disposableRef^;

let set = (newDisposable, {disposableRef} as assignableDisposable) =>
  if (assignableDisposable |> isDisposed) {
    newDisposable |> Disposable.dispose;
  } else {
    Interlocked.exchange(newDisposable, disposableRef) |> Disposable.dispose;
  };
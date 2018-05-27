type t = {
  disposableRef: ref(Disposable.t),
  disposable: Disposable.t,
};

let create = () => {
  let disposableRef = ref(Disposable.disposed);
  {
    disposableRef,
    disposable:
      Disposable.create(() =>
        Interlocked.exchange(Disposable.disposed, disposableRef)
        |> Disposable.dispose
      ),
  };
};

let dispose = ({disposable}) => disposable |> Disposable.dispose;

let isDisposed = ({disposable}) => disposable |> Disposable.isDisposed;

let set = (newDisposable, {disposableRef, disposable}) =>
  if (disposable |> Disposable.isDisposed) {
    disposable |> Disposable.dispose;
  } else {
    Interlocked.exchange(newDisposable, disposableRef) |> Disposable.dispose;
  };

let toDisposable = ({disposable}) => disposable;
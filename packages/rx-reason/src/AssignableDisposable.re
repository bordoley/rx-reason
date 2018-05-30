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

let set = (newDisposable, assignableDisposable) =>
  if (assignableDisposable |> isDisposed) {
    newDisposable |> Disposable.dispose;
  } else {
    Interlocked.exchange(newDisposable, assignableDisposable.disposableRef)
    |> Disposable.dispose;
  };

let toDisposable = ({disposable}) => disposable;
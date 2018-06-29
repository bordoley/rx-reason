type t = 
  | Disposed
  | SerialDisposable(Atomic.t(Disposable.t), Disposable.t);

type serialDisposable = t;

module type S = {
  type t;

  include SerialDisposableLike.S with type t := t;
  include Disposable.S with type t := t;

  let asSerialDisposable: t => serialDisposable;
};

module type S1 = {
  type t('a);

  include SerialDisposableLike.S1 with type t('a) := t('a);
  include Disposable.S1 with type t('a) := t('a);

  let asSerialDisposable: t('a) => serialDisposable;
};

let asDisposable = fun 
| Disposed => Disposable.disposed
| SerialDisposable(_, disposable) => disposable;

let create = {
  let teardown = innerDisposable =>
    Atomic.exchange(innerDisposable, Disposable.disposed)
    |> Disposable.dispose;

  () => {
    let innerDisposable = Atomic.make(Disposable.disposed);
    let disposable = Disposable.create1(teardown, innerDisposable);
    SerialDisposable(innerDisposable, disposable);
  };
};

let dispose = disposable => 
  disposable |> asDisposable |> Disposable.dispose;

let disposed = Disposed;

let isDisposed = disposable => 
  disposable |> asDisposable |> Disposable.isDisposed;

let raiseIfDisposed = disposable => 
  disposable |> asDisposable |> Disposable.raiseIfDisposed;

let getInnerDisposable = fun
| Disposed => Disposable.disposed
| SerialDisposable(innerDisposable, _) => Atomic.get(innerDisposable);

let setInnerDisposable = (newDisposable, self) => {
  let shouldDispose = switch(self) {
  | Disposed => true;
  | SerialDisposable(innerDisposable, _) =>
    Atomic.exchange(innerDisposable, newDisposable) |> Disposable.dispose;
    isDisposed(self);
  };

  if (shouldDispose) {
    newDisposable |> Disposable.dispose;
  };
};
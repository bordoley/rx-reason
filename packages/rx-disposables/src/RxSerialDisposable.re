type t =
  | Disposed
  | SerialDisposable(RxAtomic.t(RxDisposable.t), RxDisposable.t);

type serialDisposable = t;

module type S = {
  type t;

  include RxSerialDisposableLike.S with type t := t;
  include RxDisposable.S with type t := t;

  let asSerialDisposable: t => serialDisposable;
};

module type S1 = {
  type t('a);

  include RxSerialDisposableLike.S1 with type t('a) := t('a);
  include RxDisposable.S1 with type t('a) := t('a);

  let asSerialDisposable: t('a) => serialDisposable;
};

let asDisposable =
  fun
  | Disposed => RxDisposable.disposed
  | SerialDisposable(_, disposable) => disposable;

let create = {
  let teardown = innerDisposable =>
  RxAtomic.exchange(innerDisposable, RxDisposable.disposed)
    |> RxDisposable.dispose;

  () => {
    let innerDisposable = RxAtomic.make(RxDisposable.disposed);
    let disposable = RxDisposable.create1(teardown, innerDisposable);
    SerialDisposable(innerDisposable, disposable);
  };
};

let dispose = disposable => disposable |> asDisposable |> RxDisposable.dispose;

let disposed = Disposed;

let isDisposed = disposable =>
  disposable |> asDisposable |> RxDisposable.isDisposed;

let raiseIfDisposed = disposable =>
  disposable |> asDisposable |> RxDisposable.raiseIfDisposed;

let getInnerDisposable =
  fun
  | Disposed => RxDisposable.disposed
  | SerialDisposable(innerDisposable, _) => RxAtomic.get(innerDisposable);

let setInnerDisposable = (newDisposable, self) => {
  let shouldDispose =
    switch (self) {
    | Disposed => true
    | SerialDisposable(innerDisposable, _) =>
      let oldDisposable = RxAtomic.exchange(innerDisposable, newDisposable);
      if (oldDisposable !== newDisposable) {
        oldDisposable |> RxDisposable.dispose;
      };
      isDisposed(self);
    };

  if (shouldDispose) {
    newDisposable |> RxDisposable.dispose;
  };
};
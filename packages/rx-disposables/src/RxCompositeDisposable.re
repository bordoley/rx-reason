type t =
  | Disposed
  | CompositeDisposable(
      RxLock.t,
      RxMutableList.t(RxDisposable.t),
      RxDisposable.t,
    );

type compositeDisposable = t;

module type S = {
  type t;

  include RxCompositeDisposableLike.S with type t := t;
  include RxDisposable.S with type t := t;

  let asCompositeDisposable: t => compositeDisposable;
};

module type S1 = {
  type t('a);

  include RxCompositeDisposableLike.S1 with type t('a) := t('a);
  include RxDisposable.S1 with type t('a) := t('a);

  let asCompositeDisposable: t('a) => compositeDisposable;
};

let asDisposable =
  fun
  | Disposed => RxDisposable.disposed
  | CompositeDisposable(_, _, disposable) => disposable;

let disposed = Disposed;

let create = {
  let teardown = (lock, teardown) => {
    lock |> RxLock.acquire;
    teardown |> RxMutableList.forEachReversed(RxDisposable.dispose);
    teardown |> RxMutableList.clear;
    lock |> RxLock.release;
  };

  () => {
    let children = RxMutableList.create();
    let lock = RxLock.create();
    let disposable = RxDisposable.create2(teardown, lock, children);
    CompositeDisposable(lock, children, disposable);
  };
};

let dispose = disposable => disposable |> asDisposable |> RxDisposable.dispose;

let isDisposed = disposable =>
  disposable |> asDisposable |> RxDisposable.isDisposed;

let raiseIfDisposed = disposable =>
  disposable |> asDisposable |> RxDisposable.raiseIfDisposed;

let addDisposable = (disposable, self) => {
  switch (self) {
  | Disposed => ()
  | CompositeDisposable(lock, children, _) =>
    lock |> RxLock.acquire;

    let isDisposed = isDisposed(self);
    if (! isDisposed) {
      children |> RxMutableList.add(disposable);
      lock |> RxLock.release;
    }
    else {
      lock |> RxLock.release;
      disposable |> RxDisposable.dispose;
    };
  };
  self;
};

let removeDisposable = (disposable, self) => {
  switch (self) {
  | Disposed => ()
  | CompositeDisposable(lock, children, _) =>
    lock |> RxLock.acquire;

    let isDisposed = isDisposed(self);
    if (! isDisposed) {
      children |> RxMutableList.remove(disposable);
    };

    lock |> RxLock.release;
  };
  self;
};
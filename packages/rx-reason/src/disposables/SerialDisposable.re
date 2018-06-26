type t = {
  disposableRef: ref(Disposable.t),
  disposable: Disposable.t,
};

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

let disposed = {
  disposableRef: ref(Disposable.disposed),
  disposable: Disposable.disposed,
};

let isDisposed = ({disposable}) => disposable |> Disposable.isDisposed;

let raiseIfDisposed = ({disposable}) =>
  disposable |> Disposable.raiseIfDisposed;

let getInnerDisposable = ({disposableRef}) => disposableRef^;

let setInnerDisposable =
    (newDisposable, {disposableRef} as assignableDisposable) =>
  if (assignableDisposable |> isDisposed) {
    newDisposable |> Disposable.dispose;
  } else {
    Interlocked.exchange(newDisposable, disposableRef) |> Disposable.dispose;
  };
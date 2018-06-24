type t = {
  disposableRef: ref(Disposable.t),
  disposable: Disposable.t,
};

type serialDisposable = t;

module type S = {
  type t;

  include Disposable.S with type t := t;

  let asSerialDisposable: t => serialDisposable;

  /** Returns the currently contained Disposable */
  let get: t => Disposable.t;

  /** Atomically set the next disposable on this container and dispose the previous
   *  one (if any) or dispose next if the container has been disposed
   */
  let set: (Disposable.t, t) => unit;
};


module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let asSerialDisposable: t('a) => serialDisposable;
  
  /** Returns the currently contained Disposable */
  let get: t('a) => Disposable.t;

  /** Atomically set the next disposable on this container and dispose the previous
   *  one (if any) or dispose next if the container has been disposed
   */
  let set: (Disposable.t, t('a)) => unit;
};

let asDisposable = ({disposable}) => disposable;

let asSerialDisposable = Functions.identity;

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

let get = ({disposableRef}) => disposableRef^;

let set = (newDisposable, {disposableRef} as assignableDisposable) =>
  if (assignableDisposable |> isDisposed) {
    newDisposable |> Disposable.dispose;
  } else {
    Interlocked.exchange(newDisposable, disposableRef) |> Disposable.dispose;
  };
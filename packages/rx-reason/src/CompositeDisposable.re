type t = {
  lock: Lock.t,
  teardown: ref(list(unit => unit)),
  disposable: Disposable.t,
};

type compositeDisposable = t;

module type S = {
  type t;

  include Disposable.S with type t := t;

  let addDisposable: (Disposable.t, t) => t;

  let addTeardown: (unit => unit, t) => t;

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t => compositeDisposable;
};

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let addDisposable: (Disposable.t, t('a)) => t('a);

  let addTeardown: (unit => unit, t('a)) => t('a);

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t('a) => compositeDisposable;
};

let asCompositeDisposable = Functions.identity;

let asDisposable = ({disposable}) => disposable;

let create = () => {
  let lock = Lock.create();
  let teardown = ref([]);
  {
    lock,
    teardown,
    disposable:
      Disposable.create(() => {
        lock |> Lock.acquire;
        let teardown = Interlocked.exchange([], teardown);
        teardown |> List.iter(f => f());
        lock |> Lock.release;
      }),
  };
};

let dispose = ({disposable}) => disposable |> Disposable.dispose;

let disposed = {
  let disposedLock = Lock.create();
  let disposedTeardown = ref([]);
  {
    lock: disposedLock,
    teardown: disposedTeardown,
    disposable: Disposable.disposed,
  };
};

let isDisposed = ({disposable}) => disposable |> Disposable.isDisposed;

let raiseIfDisposed = ({disposable}) =>
  disposable |> Disposable.raiseIfDisposed;

let addTeardown = (cb, {lock, teardown} as disposable) => {
  if (cb !== TeardownLogic.none) {
    lock |> Lock.acquire;
    let isDisposed = disposable |> isDisposed;

    if (! isDisposed) {
      teardown := [cb, ...teardown^];
    };
    lock |> Lock.release;

    if (isDisposed) {
      cb();
    };
  };

  disposable;
};

let addDisposable = (disposable, self) =>
  if (Disposable.isDisposed(disposable)) {
    self
  } else {
    self |> addTeardown(() => Disposable.dispose(disposable));
  };
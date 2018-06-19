type teardownLogicOrDisposable =
  | TeardownLogic(TeardownLogic.t)
  | Disposable(Disposable.t);

type t = {
  lock: Lock.t,
  mutable teardown: list(teardownLogicOrDisposable),
  disposable: Disposable.t,
};

type compositeDisposable = t;

module type S = {
  type t;

  include Disposable.S with type t := t;

  let addDisposable: (Disposable.t, t) => t;

  let addTeardown: (unit => unit, t) => t;

  let asCompositeDisposable: t => compositeDisposable;
};

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let addDisposable: (Disposable.t, t('a)) => t('a);

  let addTeardown: (unit => unit, t('a)) => t('a);

  let asCompositeDisposable: t('a) => compositeDisposable;
};

let asCompositeDisposable = Functions.identity;

let asDisposable = ({disposable}) => disposable;

let disposed = {
  let lock = Lock.create();
  {lock, teardown: [], disposable: Disposable.disposed};
};

let create = () => {
  let retval = ref(disposed);
  let lock = Lock.create();

  let teardown = () => {
    lock |> Lock.acquire;
    let teardown = retval^.teardown;
    teardown
    |> List.iter(ele =>
         switch (ele) {
         | TeardownLogic(tdl) => tdl()
         | Disposable(disposable) => disposable |> Disposable.dispose
         }
       );
    lock |> Lock.release;
  };

  retval := {lock, teardown: [], disposable: Disposable.create(teardown)};
  retval^;
};

let dispose = ({disposable}) => disposable |> Disposable.dispose;

let isDisposed = ({disposable}) => disposable |> Disposable.isDisposed;

let raiseIfDisposed = ({disposable}) =>
  disposable |> Disposable.raiseIfDisposed;

let addInternal = (teardownLogicOrDisposable, {lock, teardown} as disposable) => {
  lock |> Lock.acquire;
  let isDisposed = disposable |> isDisposed;

  if (! isDisposed) {
    disposable.teardown = [teardownLogicOrDisposable, ...teardown];
  };
  lock |> Lock.release;
  isDisposed;
};

let addTeardown = (cb, self) => {
  if (cb !== TeardownLogic.none) {
    let isDisposed = self |> addInternal(TeardownLogic(cb));
    if (isDisposed) {
      cb();
    };
  };

  self;
};

let addDisposable = (disposable, self) => {
  if (! Disposable.isDisposed(disposable)) {
    let isDisposed = self |> addInternal(Disposable(disposable));
    if (isDisposed) {
      disposable |> Disposable.dispose;
    };
  };

  self;
};
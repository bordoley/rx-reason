type teardownLogicOrDisposable =
  | TeardownLogic(TeardownLogic.t): teardownLogicOrDisposable
  | TeardownLogic1(TeardownLogic.t1('a), 'a): teardownLogicOrDisposable
  | TeardownLogic2(TeardownLogic.t2('a, 'b), 'a, 'b): teardownLogicOrDisposable
  | Disposable(Disposable.t);

type t = {
  lock: Lock.t,
  /* FIXME: Change to mutable vector and profit */
  mutable teardown: list(teardownLogicOrDisposable),
  disposable: Disposable.t,
};

type compositeDisposable = t;

module type S = {
  type t;

  include Disposable.S with type t := t;

  let addDisposable: (Disposable.t, t) => t;

  let addTeardown: (TeardownLogic.t, t) => t;

  let addTeardown1: (TeardownLogic.t1('b), 'b, t) => t;

  let addTeardown2: (TeardownLogic.t2('b, 'c), 'b, 'c, t) => t;

  let asCompositeDisposable: t => compositeDisposable;
};

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let addDisposable: (Disposable.t, t('a)) => t('a);

  let addTeardown: (TeardownLogic.t, t('a)) => t('a);

  let addTeardown1: (TeardownLogic.t1('b), 'b, t('a)) => t('a);

  let addTeardown2: (TeardownLogic.t2('b, 'c), 'b, 'c, t('a)) => t('a);

  let asCompositeDisposable: t('a) => compositeDisposable;
};

let asCompositeDisposable = Functions.identity;

let asDisposable = ({disposable}) => disposable;

let disposed = {
  let lock = Lock.create();
  {lock, teardown: [], disposable: Disposable.disposed};
};

let create = {
  let teardown = (lock, self) => {
    lock |> Lock.acquire;
    let teardown = self^.teardown;
    teardown
    |> List.iter(
         fun
         | TeardownLogic(tdl) => tdl()
         | TeardownLogic1(tdl, d0) => tdl(d0)
         | TeardownLogic2(tdl, d0, d1) => tdl(d0, d1)
         | Disposable(disposable) => disposable |> Disposable.dispose,
       );
    lock |> Lock.release;
  };

  () => {
    let retval = ref(disposed);
    let lock = Lock.create();

    retval :=
      {
        lock,
        teardown: [],
        disposable: Disposable.create2(teardown, lock, retval),
      };
    retval^;
  };
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
  let isDisposed = self |> addInternal(TeardownLogic(cb));
  if (isDisposed) {
    cb();
  };

  self;
};

let addTeardown1 = (cb, d0, self) => {
  let isDisposed = self |> addInternal(TeardownLogic1(cb, d0));
  if (isDisposed) {
    cb(d0);
  };

  self;
};

let addTeardown2 = (cb, d0, d1, self) => {
  let isDisposed = self |> addInternal(TeardownLogic2(cb, d0, d1));
  if (isDisposed) {
    cb(d0, d1);
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
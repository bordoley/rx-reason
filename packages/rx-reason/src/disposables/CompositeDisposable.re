type teardownLogicOrDisposable =
  | TeardownLogic(TeardownLogic.t): teardownLogicOrDisposable
  | TeardownLogic1(TeardownLogic.t1('a), 'a): teardownLogicOrDisposable
  | TeardownLogic2(TeardownLogic.t2('a, 'b), 'a, 'b): teardownLogicOrDisposable
  | TeardownLogic3(TeardownLogic.t3('a, 'b, 'c), 'a, 'b, 'c): teardownLogicOrDisposable
  | TeardownLogic4(TeardownLogic.t4('a, 'b, 'c, 'd), 'a, 'b, 'c, 'd): teardownLogicOrDisposable
  | TeardownLogic5(TeardownLogic.t5('a, 'b, 'c, 'd, 'e), 'a, 'b, 'c, 'd, 'e): teardownLogicOrDisposable
  | TeardownLogic6(
                    TeardownLogic.t6('a, 'b, 'c, 'd, 'e, 'f),
                    'a,
                    'b,
                    'c,
                    'd,
                    'e,
                    'f,
                  ): teardownLogicOrDisposable
  | TeardownLogic7(
                    TeardownLogic.t7('a, 'b, 'c, 'd, 'e, 'f, 'g),
                    'a,
                    'b,
                    'c,
                    'd,
                    'e,
                    'f,
                    'g,
                  ): teardownLogicOrDisposable
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

  include CompositeDisposableLike.S with type t := t;
  include Disposable.S with type t := t;

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t => compositeDisposable;
};

module type S1 = {
  type t('a);

  include CompositeDisposableLike.S1 with type t('a) := t('a);
  include Disposable.S1 with type t('a) := t('a);

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t('a) => compositeDisposable;
};

let asDisposable = ({disposable}) => disposable;

let disposed = {
  let lock = Lock.create();
  {lock, teardown: [], disposable: Disposable.disposed};
};

let create = {
  let teardown = {
    let doTeardown =
      fun
      | TeardownLogic(tdl) => tdl()
      | TeardownLogic1(tdl, d0) => tdl(d0)
      | TeardownLogic2(tdl, d0, d1) => tdl(d0, d1)
      | TeardownLogic3(tdl, d0, d1, d2) => tdl(d0, d1, d2)
      | TeardownLogic4(tdl, d0, d1, d2, d3) => tdl(d0, d1, d2, d3)
      | TeardownLogic5(tdl, d0, d1, d2, d3, d4) => tdl(d0, d1, d2, d3, d4)
      | TeardownLogic6(tdl, d0, d1, d2, d3, d4, d5) =>
        tdl(d0, d1, d2, d3, d4, d5)
      | TeardownLogic7(tdl, d0, d1, d2, d3, d4, d5, d6) =>
        tdl(d0, d1, d2, d3, d4, d5, d6)
      | Disposable(disposable) => disposable |> Disposable.dispose;

    (lock, self) => {
      lock |> Lock.acquire;
      let teardown = self^.teardown;
      teardown |> List.iter(doTeardown);
      lock |> Lock.release;
    };
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

let dispose = disposable => disposable |> asDisposable |> Disposable.dispose;

let isDisposed = disposable =>
  disposable |> asDisposable |> Disposable.isDisposed;

let raiseIfDisposed = disposable =>
  disposable |> asDisposable |> Disposable.raiseIfDisposed;

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

let addTeardown3 = (cb, d0, d1, d2, self) => {
  let isDisposed = self |> addInternal(TeardownLogic3(cb, d0, d1, d2));
  if (isDisposed) {
    cb(d0, d1, d2);
  };

  self;
};

let addTeardown4 = (cb, d0, d1, d2, d3, self) => {
  let isDisposed = self |> addInternal(TeardownLogic4(cb, d0, d1, d2, d3));
  if (isDisposed) {
    cb(d0, d1, d2, d3);
  };

  self;
};

let addTeardown5 = (cb, d0, d1, d2, d3, d4, self) => {
  let isDisposed =
    self |> addInternal(TeardownLogic5(cb, d0, d1, d2, d3, d4));
  if (isDisposed) {
    cb(d0, d1, d2, d3, d4);
  };

  self;
};

let addTeardown6 = (cb, d0, d1, d2, d3, d4, d5, self) => {
  let isDisposed =
    self |> addInternal(TeardownLogic6(cb, d0, d1, d2, d3, d4, d5));
  if (isDisposed) {
    cb(d0, d1, d2, d3, d4, d5);
  };

  self;
};

let addTeardown7 = (cb, d0, d1, d2, d3, d4, d5, d6, self) => {
  let isDisposed =
    self |> addInternal(TeardownLogic7(cb, d0, d1, d2, d3, d4, d5, d6));
  if (isDisposed) {
    cb(d0, d1, d2, d3, d4, d5, d6);
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
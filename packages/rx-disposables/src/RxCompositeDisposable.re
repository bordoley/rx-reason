type teardownLogic =
  | TeardownLogic(RxDisposableTeardownLogic.t): teardownLogic
  | TeardownLogic1(RxDisposableTeardownLogic.t1('a), 'a): teardownLogic
  | TeardownLogic2(RxDisposableTeardownLogic.t2('a, 'b), 'a, 'b): teardownLogic
  | TeardownLogic3(RxDisposableTeardownLogic.t3('a, 'b, 'c), 'a, 'b, 'c): teardownLogic
  | TeardownLogic4(
                    RxDisposableTeardownLogic.t4('a, 'b, 'c, 'd),
                    'a,
                    'b,
                    'c,
                    'd,
                  ): teardownLogic
  | TeardownLogic5(
                    RxDisposableTeardownLogic.t5('a, 'b, 'c, 'd, 'e),
                    'a,
                    'b,
                    'c,
                    'd,
                    'e,
                  ): teardownLogic
  | TeardownLogic6(
                    RxDisposableTeardownLogic.t6('a, 'b, 'c, 'd, 'e, 'f),
                    'a,
                    'b,
                    'c,
                    'd,
                    'e,
                    'f,
                  ): teardownLogic
  | TeardownLogic7(
                    RxDisposableTeardownLogic.t7('a, 'b, 'c, 'd, 'e, 'f, 'g),
                    'a,
                    'b,
                    'c,
                    'd,
                    'e,
                    'f,
                    'g,
                  ): teardownLogic;

type t =
  | Disposed
  | CompositeDisposable(
      RxLock.t,
      RxMutableQueue.t(teardownLogic),
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
  let teardown = {
    let doTeardown =
      fun
      | TeardownLogic(tdl) => tdl()
      | TeardownLogic1(tdl, d0) => tdl(d0)
      | TeardownLogic2(tdl, d0, d1) => tdl(d0, d1)
      | TeardownLogic3(tdl, d0, d1, d2) => tdl(d0, d1, d2)
      | TeardownLogic4(tdl, d0, d1, d2, d3) => tdl(d0, d1, d2, d3)
      | TeardownLogic5(tdl, d0, d1, d2, d3, d4) =>
        tdl(d0, d1, d2, d3, d4)
      | TeardownLogic6(tdl, d0, d1, d2, d3, d4, d5) =>
        tdl(d0, d1, d2, d3, d4, d5)
      | TeardownLogic7(tdl, d0, d1, d2, d3, d4, d5, d6) =>
        tdl(d0, d1, d2, d3, d4, d5, d6);

    (lock, teardown) => {
      lock |> RxLock.acquire;
      teardown |> RxMutableQueue.forEach(doTeardown);
      teardown |> RxMutableQueue.clear;
      lock |> RxLock.release;
    };
  };

  () => {
    let children = RxMutableQueue.create();
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

let addInternal = (teardownLogic, self) =>
  switch (self) {
  | Disposed => true
  | CompositeDisposable(lock, children, _) =>
    lock |> RxLock.acquire;

    let isDisposed = isDisposed(self);
    if (! isDisposed) {
      children |> RxMutableQueue.enqueue(teardownLogic);
    };

    lock |> RxLock.release;
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
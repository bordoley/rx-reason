type t =
  | Disposable(RxAtomic.t(bool), unit => unit)
  | Disposable1(RxAtomic.t(bool), 'ctx0 => unit, 'ctx0): t
  | Disposable2(RxAtomic.t(bool), ('ctx0, 'ctx1) => unit, 'ctx0, 'ctx1): t
  | Disposable3(
                 RxAtomic.t(bool),
                 ('ctx0, 'ctx1, 'ctx2) => unit,
                 'ctx0,
                 'ctx1,
                 'ctx2,
               ): t
  | Disposable4(
                 RxAtomic.t(bool),
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3) => unit,
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
               ): t
  | Disposable5(
                 RxAtomic.t(bool),
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4) => unit,
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
               ): t
  | Disposed
  | Empty(RxAtomic.t(bool));

type disposable = t;

module type S = {
  include RxDisposableLike.S;

  let asDisposable: t => disposable;
};

module type S1 = {
  include RxDisposableLike.S1;

  let asDisposable: t('a) => disposable;
};

let create = teardown : t => Disposable(RxAtomic.make(false), teardown);

let create1 = (teardown, d0) : t =>
  Disposable1(RxAtomic.make(false), teardown, d0);

let create2 = (teardown, d0, d1) : t =>
  Disposable2(RxAtomic.make(false), teardown, d0, d1);

let create3 = (teardown, d0, d1, d2) : t =>
  Disposable3(RxAtomic.make(false), teardown, d0, d1, d2);

let create4 = (teardown, d0, d1, d2, d3) : t =>
  Disposable4(RxAtomic.make(false), teardown, d0, d1, d2, d3);

let create5 = (teardown, d0, d1, d2, d3, d4) : t =>
  Disposable5(RxAtomic.make(false), teardown, d0, d1, d2, d3, d4);

let empty = () => Empty(RxAtomic.make(false));

let disposed: t = Disposed;

let dispose = {
  let shouldDispose =
    fun
    | Disposable(isDisposed, _)
    | Disposable1(isDisposed, _, _)
    | Disposable2(isDisposed, _, _, _)
    | Disposable3(isDisposed, _, _, _, _)
    | Disposable4(isDisposed, _, _, _, _, _)
    | Disposable5(isDisposed, _, _, _, _, _, _)
    | Empty(isDisposed) => ! RxAtomic.exchange(isDisposed, true)
    | Disposed => false;

  let doTeardown =
    fun
    | Disposable(_, teardown) => teardown()
    | Disposable1(_, teardown, a0) => teardown(a0)
    | Disposable2(_, teardown, a0, a1) => teardown(a0, a1)
    | Disposable3(_, teardown, a0, a1, a2) => teardown(a0, a1, a2)
    | Disposable4(_, teardown, a0, a1, a2, a3) => teardown(a0, a1, a2, a3)
    | Disposable5(_, teardown, a0, a1, a2, a3, a4) =>
      teardown(a0, a1, a2, a3, a4)
    | Empty(_)
    | Disposed => ();

  disposable => {
    let shouldDispose = shouldDispose(disposable);
    if (shouldDispose) {
      try( doTeardown(disposable)) {
      /* Proactively catch exceptions thrown in teardown logic. Teardown functions
       * shouldn't throw, so this is to proactively prevent unexpected exceptions.
       */
      | _ => ()
      }
    };
  };
};

let compose = {
  let rec disposeAll = list =>
    switch (list) {
    | [] => ()
    | [hd, ...tail] =>
      dispose(hd);
      disposeAll(tail);
    };

  disposables => create1(disposeAll, disposables);
};

let isDisposed =
  fun
  | Disposable(isDisposed, _)
  | Disposable1(isDisposed, _, _)
  | Disposable2(isDisposed, _, _, _)
  | Disposable3(isDisposed, _, _, _, _)
  | Disposable4(isDisposed, _, _, _, _, _)
  | Disposable5(isDisposed, _, _, _, _, _, _)
  | Empty(isDisposed) => RxAtomic.get(isDisposed)
  | Disposed => true;
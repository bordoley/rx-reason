type t('a) =
  | AutoDisposing(
      CompositeDisposable.t,
      Atomic.t(bool),
      'a => unit,
      option(exn) => unit,
    )
  | AutoDisposing1(
                    CompositeDisposable.t,
                    Atomic.t(bool),
                    'ctx0,
                    ('ctx0, 'a) => unit,
                    ('ctx0, option(exn)) => unit,
                  ): t('a)
  | AutoDisposing2(
                    CompositeDisposable.t,
                    Atomic.t(bool),
                    'ctx0,
                    'ctx1,
                    ('ctx0, 'ctx1, 'a) => unit,
                    ('ctx0, 'ctx1, option(exn)) => unit,
                  ): t('a)
  | AutoDisposing3(
                    CompositeDisposable.t,
                    Atomic.t(bool),
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
                  ): t('a)
  | AutoDisposing4(
                    CompositeDisposable.t,
                    Atomic.t(bool),
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    'ctx3,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
                  ): t('a)
  | AutoDisposing5(
                    CompositeDisposable.t,
                    Atomic.t(bool),
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    'ctx3,
                    'ctx4,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit,
                  ): t('a)
  | AutoDisposing6(
                    CompositeDisposable.t,
                    Atomic.t(bool),
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    'ctx3,
                    'ctx4,
                    'ctx5,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, option(exn)) =>
                    unit,
                  ): t('a)
  | AutoDisposing7(
                    CompositeDisposable.t,
                    Atomic.t(bool),
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    'ctx3,
                    'ctx4,
                    'ctx5,
                    'ctx6,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'a) =>
                    unit,
                    (
                      'ctx0,
                      'ctx1,
                      'ctx2,
                      'ctx3,
                      'ctx4,
                      'ctx5,
                      'ctx6,
                      option(exn)
                    ) =>
                    unit,
                  ): t('a)
  | Decorating(
                t('b),
                Atomic.t(bool),
                (t('b), 'a) => unit,
                (t('b), option(exn)) => unit,
              ): t('a)
  | Decorating1(
                 t('b),
                 Atomic.t(bool),
                 'ctx0,
                 ('ctx0, t('b), 'a) => unit,
                 ('ctx0, t('b), option(exn)) => unit,
               ): t('a)
  | Decorating2(
                 t('b),
                 Atomic.t(bool),
                 'ctx0,
                 'ctx1,
                 ('ctx0, 'ctx1, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, t('b), option(exn)) => unit,
               ): t('a)
  | Decorating3(
                 t('b),
                 Atomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 ('ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
               ): t('a)
  | Decorating4(
                 t('b),
                 Atomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), option(exn)) => unit,
               ): t('a)
  | Decorating5(
                 t('b),
                 Atomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), option(exn)) =>
                 unit,
               ): t('a)
  | Decorating6(
                 t('b),
                 Atomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 'ctx5,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, t('b), 'a) => unit,
                 (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   t('b),
                   option(exn)
                 ) =>
                 unit,
               ): t('a)
  | Decorating7(
                 t('b),
                 Atomic.t(bool),
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 'ctx5,
                 'ctx6,
                 (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   'ctx6,
                   t('b),
                   'a
                 ) =>
                 unit,
                 (
                   'ctx0,
                   'ctx1,
                   'ctx2,
                   'ctx3,
                   'ctx4,
                   'ctx5,
                   'ctx6,
                   t('b),
                   option(exn)
                 ) =>
                 unit,
               ): t('a)
  | Disposed;

let rec asCompositeDisposable: type a. t(a) => CompositeDisposable.t =
  fun
  | AutoDisposing(disposable, _, _, _) => disposable
  | AutoDisposing1(disposable, _, _, _, _) => disposable
  | AutoDisposing2(disposable, _, _, _, _, _) => disposable
  | AutoDisposing3(disposable, _, _, _, _, _, _) => disposable
  | AutoDisposing4(disposable, _, _, _, _, _, _, _) => disposable
  | AutoDisposing5(disposable, _, _, _, _, _, _, _, _) => disposable
  | AutoDisposing6(disposable, _, _, _, _, _, _, _, _, _) => disposable
  | AutoDisposing7(disposable, _, _, _, _, _, _, _, _, _, _) => disposable
  | Decorating(delegate, _, _, _) => delegate |> asCompositeDisposable
  | Decorating1(delegate, _, _, _, _) => delegate |> asCompositeDisposable
  | Decorating2(delegate, _, _, _, _, _) => delegate |> asCompositeDisposable
  | Decorating3(delegate, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating4(delegate, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating5(delegate, _, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating6(delegate, _, _, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Decorating7(delegate, _, _, _, _, _, _, _, _, _, _) =>
    delegate |> asCompositeDisposable
  | Disposed => CompositeDisposable.disposed;

let addTeardown = (teardown, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown(teardown)
  |> ignore;
  subscriber;
};

let addTeardown1 = (teardown, d0, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown1(teardown, d0)
  |> ignore;
  subscriber;
};

let addTeardown2 = (teardown, d0, d1, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown2(teardown, d0, d1)
  |> ignore;
  subscriber;
};

let addTeardown3 = (teardown, d0, d1, d2, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown3(teardown, d0, d1, d2)
  |> ignore;
  subscriber;
};

let addTeardown4 = (teardown, d0, d1, d2, d3, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown4(teardown, d0, d1, d2, d3)
  |> ignore;
  subscriber;
};

let addTeardown5 = (teardown, d0, d1, d2, d3, d4, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown5(teardown, d0, d1, d2, d3, d4)
  |> ignore;
  subscriber;
};

let addTeardown6 = (teardown, d0, d1, d2, d3, d4, d5, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown6(teardown, d0, d1, d2, d3, d4, d5)
  |> ignore;
  subscriber;
};

let addTeardown7 = (teardown, d0, d1, d2, d3, d4, d5, d6, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown7(teardown, d0, d1, d2, d3, d4, d5, d6)
  |> ignore;
  subscriber;
};

let asDisposable = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.asDisposable;

let create = (~onNext, ~onComplete) => {
  let isStopped = Atomic.make(false);
  let disposable =
    CompositeDisposable.create()
    |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped);
  AutoDisposing(disposable, isStopped, onNext, onComplete);
};

let create1 = (~onNext, ~onComplete, ctx0) => {
  let isStopped = Atomic.make(false);
  let disposable =
    CompositeDisposable.create()
    |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped);
  AutoDisposing1(disposable, isStopped, ctx0, onNext, onComplete);
};

let create2 = (~onNext, ~onComplete, ctx0, ctx1) => {
  let isStopped = Atomic.make(false);
  let disposable =
    CompositeDisposable.create()
    |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped);
  AutoDisposing2(disposable, isStopped, ctx0, ctx1, onNext, onComplete);
};

let create3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2) => {
  let isStopped = Atomic.make(false);
  let disposable =
    CompositeDisposable.create()
    |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped);
  AutoDisposing3(disposable, isStopped, ctx0, ctx1, ctx2, onNext, onComplete);
};

let create4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3) => {
  let isStopped = Atomic.make(false);
  let disposable =
    CompositeDisposable.create()
    |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped);
  AutoDisposing4(
    disposable,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    onNext,
    onComplete,
  );
};

let create5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4) => {
  let isStopped = Atomic.make(false);
  let disposable =
    CompositeDisposable.create()
    |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped);
  AutoDisposing5(
    disposable,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    onNext,
    onComplete,
  );
};

let create6 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) => {
  let isStopped = Atomic.make(false);
  let disposable =
    CompositeDisposable.create()
    |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped);
  AutoDisposing6(
    disposable,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    onNext,
    onComplete,
  );
};

let create7 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6) => {
  let isStopped = Atomic.make(false);
  let disposable =
    CompositeDisposable.create()
    |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped);
  AutoDisposing7(
    disposable,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
    onNext,
    onComplete,
  );
};

let decorate = (~onNext, ~onComplete, subscriber) => {
  let isStopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, isStopped) |> ignore;
  Decorating(subscriber, isStopped, onNext, onComplete);
};

let decorate1 = (~onNext, ~onComplete, ctx0, subscriber) => {
  let isStopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, isStopped) |> ignore;
  Decorating1(subscriber, isStopped, ctx0, onNext, onComplete);
};

let decorate2 = (~onNext, ~onComplete, ctx0, ctx1, subscriber) => {
  let isStopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, isStopped) |> ignore;
  Decorating2(subscriber, isStopped, ctx0, ctx1, onNext, onComplete);
};

let decorate3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, subscriber) => {
  let isStopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, isStopped) |> ignore;
  Decorating3(subscriber, isStopped, ctx0, ctx1, ctx2, onNext, onComplete);
};

let decorate4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, subscriber) => {
  let isStopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, isStopped) |> ignore;
  Decorating4(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    onNext,
    onComplete,
  );
};

let decorate5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, subscriber) => {
  let isStopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, isStopped) |> ignore;
  Decorating5(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    onNext,
    onComplete,
  );
};

let decorate6 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, subscriber) => {
  let isStopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, isStopped) |> ignore;
  Decorating6(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    onNext,
    onComplete,
  );
};

let decorate7 =
    (
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      subscriber,
    ) => {
  let isStopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, isStopped) |> ignore;
  Decorating7(
    subscriber,
    isStopped,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
    onNext,
    onComplete,
  );
};

let dispose = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.dispose;

let disposed = Disposed;

let isDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.isDisposed;

let isStopped =
  fun
  | AutoDisposing(_, isStopped, _, _)
  | AutoDisposing1(_, isStopped, _, _, _)
  | AutoDisposing2(_, isStopped, _, _, _, _)
  | AutoDisposing3(_, isStopped, _, _, _, _, _)
  | AutoDisposing4(_, isStopped, _, _, _, _, _, _)
  | AutoDisposing5(_, isStopped, _, _, _, _, _, _, _)
  | AutoDisposing6(_, isStopped, _, _, _, _, _, _, _, _)
  | AutoDisposing7(_, isStopped, _, _, _, _, _, _, _, _, _)
  | Decorating(_, isStopped, _, _)
  | Decorating1(_, isStopped, _, _, _)
  | Decorating2(_, isStopped, _, _, _, _)
  | Decorating3(_, isStopped, _, _, _, _, _)
  | Decorating4(_, isStopped, _, _, _, _, _, _)
  | Decorating5(_, isStopped, _, _, _, _, _, _, _)
  | Decorating6(_, isStopped, _, _, _, _, _, _, _, _)
  | Decorating7(_, isStopped, _, _, _, _, _, _, _, _, _) =>
    Atomic.get(isStopped)
  | Disposed => true;

let shouldComplete =
  fun
  | AutoDisposing(_, isStopped, _, _)
  | AutoDisposing1(_, isStopped, _, _, _)
  | AutoDisposing2(_, isStopped, _, _, _, _)
  | AutoDisposing3(_, isStopped, _, _, _, _, _)
  | AutoDisposing4(_, isStopped, _, _, _, _, _, _)
  | AutoDisposing5(_, isStopped, _, _, _, _, _, _, _)
  | AutoDisposing6(_, isStopped, _, _, _, _, _, _, _, _)
  | AutoDisposing7(_, isStopped, _, _, _, _, _, _, _, _, _)
  | Decorating(_, isStopped, _, _)
  | Decorating1(_, isStopped, _, _, _)
  | Decorating2(_, isStopped, _, _, _, _)
  | Decorating3(_, isStopped, _, _, _, _, _)
  | Decorating4(_, isStopped, _, _, _, _, _, _)
  | Decorating5(_, isStopped, _, _, _, _, _, _, _)
  | Decorating6(_, isStopped, _, _, _, _, _, _, _, _)
  | Decorating7(_, isStopped, _, _, _, _, _, _, _, _, _) =>
    ! Atomic.exchange(isStopped, true)
  | Disposed => false;

let completeWithResult = {
  let doComplete = (exn, subscriber) =>
    switch (subscriber) {
    | AutoDisposing(disposable, _, _, onComplete) =>
      try (onComplete(exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing1(disposable, _, ctx0, _, onComplete) =>
      try (onComplete(ctx0, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing2(disposable, _, ctx0, ctx1, _, onComplete) =>
      try (onComplete(ctx0, ctx1, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing3(disposable, _, ctx0, ctx1, ctx2, _, onComplete) =>
      try (onComplete(ctx0, ctx1, ctx2, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing4(disposable, _, ctx0, ctx1, ctx2, ctx3, _, onComplete) =>
      try (onComplete(ctx0, ctx1, ctx2, ctx3, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing5(
        disposable,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        _,
        onComplete,
      ) =>
      try (onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing6(
        disposable,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        _,
        onComplete,
      ) =>
      try (onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing7(
        disposable,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        _,
        onComplete,
      ) =>
      try (onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | Decorating(delegate, _, _, onComplete) => onComplete(delegate, exn)
    | Decorating1(delegate, _, ctx0, _, onComplete) =>
      onComplete(ctx0, delegate, exn)
    | Decorating2(delegate, _, ctx0, ctx1, _, onComplete) =>
      onComplete(ctx0, ctx1, delegate, exn)
    | Decorating3(delegate, _, ctx0, ctx1, ctx2, _, onComplete) =>
      onComplete(ctx0, ctx1, ctx2, delegate, exn)
    | Decorating4(delegate, _, ctx0, ctx1, ctx2, ctx3, _, onComplete) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, delegate, exn)
    | Decorating5(delegate, _, ctx0, ctx1, ctx2, ctx3, ctx4, _, onComplete) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, delegate, exn)
    | Decorating6(
        delegate,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        _,
        onComplete,
      ) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, delegate, exn)
    | Decorating7(
        delegate,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        _,
        onComplete,
      ) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, delegate, exn)
    | Disposed => ()
    };

  (~exn=?, subscriber) => {
    let shouldComplete = shouldComplete(subscriber);
    if (shouldComplete) {
      doComplete(exn, subscriber);
    };
    shouldComplete;
  };
};

let complete = (~exn=?, subscriber) =>
  subscriber |> completeWithResult(~exn?) |> ignore;

let next = {
  let doNext = (next, subscriber) =>
    switch (subscriber) {
    | AutoDisposing(disposable, _, onNext, _) =>
      try (onNext(next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing1(disposable, _, ctx0, onNext, _) =>
      try (onNext(ctx0, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing2(disposable, _, ctx0, ctx1, onNext, _) =>
      try (onNext(ctx0, ctx1, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing3(disposable, _, ctx0, ctx1, ctx2, onNext, _) =>
      try (onNext(ctx0, ctx1, ctx2, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing4(disposable, _, ctx0, ctx1, ctx2, ctx3, onNext, _) =>
      try (onNext(ctx0, ctx1, ctx2, ctx3, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing5(disposable, _, ctx0, ctx1, ctx2, ctx3, ctx4, onNext, _) =>
      try (onNext(ctx0, ctx1, ctx2, ctx3, ctx4, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing6(
        disposable,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        onNext,
        _,
      ) =>
      try (onNext(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing7(
        disposable,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        onNext,
        _,
      ) =>
      try (onNext(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | Decorating(delegate, _, onNext, _) => onNext(delegate, next)
    | Decorating1(delegate, _, ctx0, onNext, _) =>
      onNext(ctx0, delegate, next)
    | Decorating2(delegate, _, ctx0, ctx1, onNext, _) =>
      onNext(ctx0, ctx1, delegate, next)
    | Decorating3(delegate, _, ctx0, ctx1, ctx2, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, delegate, next)
    | Decorating4(delegate, _, ctx0, ctx1, ctx2, ctx3, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, ctx3, delegate, next)
    | Decorating5(delegate, _, ctx0, ctx1, ctx2, ctx3, ctx4, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, ctx3, ctx4, delegate, next)
    | Decorating6(delegate, _, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, onNext, _) =>
      onNext(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, delegate, next)
    | Decorating7(
        delegate,
        _,
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        onNext,
        _,
      ) =>
      onNext(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, delegate, next)
    | Disposed => ()
    };

  (next, subscriber) => {
    let isStopped = subscriber |> isStopped;
    if (! isStopped) {
      doNext(next, subscriber);
    };
  };
};

let forwardOnComplete = (subscriber, exn) => subscriber |> complete(~exn?);

let forwardOnComplete1 = (_, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete2 = (_, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete3 = (_, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete4 = (_, _, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete5 = (_, _, _, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete6 = (_, _, _, _, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnComplete7 = (_, _, _, _, _, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let forwardOnNext = (subscriber, v) => subscriber |> next(v);

let forwardOnNext1 = (_, subscriber, v) => subscriber |> next(v);

let forwardOnNext2 = (_, _, subscriber, v) => subscriber |> next(v);

let forwardOnNext3 = (_, _, _, subscriber, v) => subscriber |> next(v);

let forwardOnNext4 = (_, _, _, _, subscriber, v) => subscriber |> next(v);

let forwardOnNext5 = (_, _, _, _, _, subscriber, v) => subscriber |> next(v);

let forwardOnNext6 = (_, _, _, _, _, _, subscriber, v) =>
  subscriber |> next(v);

let forwardOnNext7 = (_, _, _, _, _, _, _, subscriber, v) =>
  subscriber |> next(v);

let notify = (notif, subscriber) =>
  switch (notif) {
  | Notification.Next(v) => subscriber |> next(v)
  | Notification.Complete(exn) => subscriber |> complete(~exn?)
  };

let raiseIfDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.raiseIfDisposed;
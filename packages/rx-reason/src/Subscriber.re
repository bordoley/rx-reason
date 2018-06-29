type t('a) =
  | AutoDisposing(
      'a => unit,
      option(exn) => unit,
      Atomic.t(bool),
      CompositeDisposable.t,
    )
  | AutoDisposing1(
                    'ctx0,
                    ('ctx0, 'a) => unit,
                    ('ctx0, option(exn)) => unit,
                    Atomic.t(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | AutoDisposing2(
                    'ctx0,
                    'ctx1,
                    ('ctx0, 'ctx1, 'a) => unit,
                    ('ctx0, 'ctx1, option(exn)) => unit,
                    Atomic.t(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | AutoDisposing3(
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
                    Atomic.t(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | AutoDisposing4(
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    'ctx3,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
                    Atomic.t(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | AutoDisposing5(
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    'ctx3,
                    'ctx4,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit,
                    Atomic.t(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | AutoDisposing6(
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    'ctx3,
                    'ctx4,
                    'ctx5,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, option(exn)) =>
                    unit,
                    Atomic.t(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | AutoDisposing7(
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
                    Atomic.t(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | Decorating(
                (t('b), 'a) => unit,
                (t('b), option(exn)) => unit,
                Atomic.t(bool),
                t('b),
              ): t('a)
  | Decorating1(
                 'ctx0,
                 ('ctx0, t('b), 'a) => unit,
                 ('ctx0, t('b), option(exn)) => unit,
                 Atomic.t(bool),
                 t('b),
               ): t('a)
  | Decorating2(
                 'ctx0,
                 'ctx1,
                 ('ctx0, 'ctx1, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, t('b), option(exn)) => unit,
                 Atomic.t(bool),
                 t('b),
               ): t('a)
  | Decorating3(
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 ('ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
                 Atomic.t(bool),
                 t('b),
               ): t('a)
  | Decorating4(
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, t('b), option(exn)) => unit,
                 Atomic.t(bool),
                 t('b),
               ): t('a)
  | Decorating5(
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 'ctx3,
                 'ctx4,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, t('b), option(exn)) =>
                 unit,
                 Atomic.t(bool),
                 t('b),
               ): t('a)
  | Decorating6(
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
                 Atomic.t(bool),
                 t('b),
               ): t('a)
  | Decorating7(
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
                 Atomic.t(bool),
                 t('b),
               ): t('a)
  | Disposed;

let rec asCompositeDisposable: type a. t(a) => CompositeDisposable.t =
  fun
  | AutoDisposing(_, _, _, disposable) => disposable
  | AutoDisposing1(_, _, _, _, disposable) => disposable
  | AutoDisposing2(_, _, _, _, _, disposable) => disposable
  | AutoDisposing3(_, _, _, _, _, _, disposable) => disposable
  | AutoDisposing4(_, _, _, _, _, _, _, disposable) => disposable
  | AutoDisposing5(_, _, _, _, _, _, _, _, disposable) => disposable
  | AutoDisposing6(_, _, _, _, _, _, _, _, _, disposable) => disposable
  | AutoDisposing7(_, _, _, _, _, _, _, _, _, _, disposable) => disposable
  | Decorating(_, _, _, delegate) => delegate |> asCompositeDisposable
  | Decorating1(_, _, _, _, delegate) => delegate |> asCompositeDisposable
  | Decorating2(_, _, _, _, _, delegate) => delegate |> asCompositeDisposable
  | Decorating3(_, _, _, _, _, _, delegate) =>
    delegate |> asCompositeDisposable
  | Decorating4(_, _, _, _, _, _, _, delegate) =>
    delegate |> asCompositeDisposable
  | Decorating5(_, _, _, _, _, _, _, _, delegate) =>
    delegate |> asCompositeDisposable
  | Decorating6(_, _, _, _, _, _, _, _, _, delegate) =>
    delegate |> asCompositeDisposable
  | Decorating7(_, _, _, _, _, _, _, _, _, _, delegate) =>
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

let createAutoDisposing = (~onNext, ~onComplete) => {
  let isStopped = Atomic.make(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped)
  |> ignore;
  AutoDisposing(onNext, onComplete, isStopped, disposable);
};

let createAutoDisposing1 = (~onNext, ~onComplete, ctx0) => {
  let isStopped = Atomic.make(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped)
  |> ignore;
  AutoDisposing1(ctx0, onNext, onComplete, isStopped, disposable);
};

let createAutoDisposing2 = (~onNext, ~onComplete, ctx0, ctx1) => {
  let isStopped = Atomic.make(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped)
  |> ignore;
  AutoDisposing2(ctx0, ctx1, onNext, onComplete, isStopped, disposable);
};

let createAutoDisposing3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2) => {
  let isStopped = Atomic.make(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped)
  |> ignore;
  AutoDisposing3(ctx0, ctx1, ctx2, onNext, onComplete, isStopped, disposable);
};

let createAutoDisposing4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3) => {
  let isStopped = Atomic.make(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped)
  |> ignore;
  AutoDisposing4(
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    onNext,
    onComplete,
    isStopped,
    disposable,
  );
};

let createAutoDisposing5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4) => {
  let isStopped = Atomic.make(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped)
  |> ignore;
  AutoDisposing5(
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    onNext,
    onComplete,
    isStopped,
    disposable,
  );
};

let createAutoDisposing6 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) => {
  let isStopped = Atomic.make(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped)
  |> ignore;
  AutoDisposing6(
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    onNext,
    onComplete,
    isStopped,
    disposable,
  );
};

let createAutoDisposing7 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6) => {
  let isStopped = Atomic.make(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Atomic.setTrue, isStopped)
  |> ignore;
  AutoDisposing7(
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
    onNext,
    onComplete,
    isStopped,
    disposable,
  );
};

let decorate = (~onNext, ~onComplete, subscriber) => {
  let stopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, stopped) |> ignore;
  Decorating(onNext, onComplete, stopped, subscriber);
};

let decorate1 = (~onNext, ~onComplete, ctx0, subscriber) => {
  let stopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, stopped) |> ignore;
  Decorating1(ctx0, onNext, onComplete, stopped, subscriber);
};

let decorate2 = (~onNext, ~onComplete, ctx0, ctx1, subscriber) => {
  let stopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, stopped) |> ignore;
  Decorating2(ctx0, ctx1, onNext, onComplete, stopped, subscriber);
};

let decorate3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, subscriber) => {
  let stopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, stopped) |> ignore;
  Decorating3(ctx0, ctx1, ctx2, onNext, onComplete, stopped, subscriber);
};

let decorate4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, subscriber) => {
  let stopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, stopped) |> ignore;
  Decorating4(
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    onNext,
    onComplete,
    stopped,
    subscriber,
  );
};

let decorate5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, subscriber) => {
  let stopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, stopped) |> ignore;
  Decorating5(
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    onNext,
    onComplete,
    stopped,
    subscriber,
  );
};

let decorate6 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, subscriber) => {
  let stopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, stopped) |> ignore;
  Decorating6(
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    onNext,
    onComplete,
    stopped,
    subscriber,
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
  let stopped = Atomic.make(false);
  subscriber |> addTeardown1(Atomic.setTrue, stopped) |> ignore;
  Decorating7(
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
    onNext,
    onComplete,
    stopped,
    subscriber,
  );
};

let dispose = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.dispose;

let disposed = Disposed;

let isDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.isDisposed;

let isStopped =
  fun
  | AutoDisposing(_, _, stopped, _)
  | AutoDisposing1(_, _, _, stopped, _)
  | AutoDisposing2(_, _, _, _, stopped, _)
  | AutoDisposing3(_, _, _, _, _, stopped, _)
  | AutoDisposing4(_, _, _, _, _, _, stopped, _)
  | AutoDisposing5(_, _, _, _, _, _, _, stopped, _)
  | AutoDisposing6(_, _, _, _, _, _, _, _, stopped, _)
  | AutoDisposing7(_, _, _, _, _, _, _, _, _, stopped, _)
  | Decorating(_, _, stopped, _)
  | Decorating1(_, _, _, stopped, _)
  | Decorating2(_, _, _, _, stopped, _)
  | Decorating3(_, _, _, _, _, stopped, _)
  | Decorating4(_, _, _, _, _, _, stopped, _)
  | Decorating5(_, _, _, _, _, _, _, stopped, _)
  | Decorating6(_, _, _, _, _, _, _, _, stopped, _)
  | Decorating7(_, _, _, _, _, _, _, _, _, stopped, _) =>
    Atomic.get(stopped)
  | Disposed => true;

let shouldComplete =
  fun
  | AutoDisposing(_, _, stopped, _)
  | AutoDisposing1(_, _, _, stopped, _)
  | AutoDisposing2(_, _, _, _, stopped, _)
  | AutoDisposing3(_, _, _, _, _, stopped, _)
  | AutoDisposing4(_, _, _, _, _, _, stopped, _)
  | AutoDisposing5(_, _, _, _, _, _, _, stopped, _)
  | AutoDisposing6(_, _, _, _, _, _, _, _, stopped, _)
  | AutoDisposing7(_, _, _, _, _, _, _, _, _, stopped, _)
  | Decorating(_, _, stopped, _)
  | Decorating1(_, _, _, stopped, _)
  | Decorating2(_, _, _, _, stopped, _)
  | Decorating3(_, _, _, _, _, stopped, _)
  | Decorating4(_, _, _, _, _, _, stopped, _)
  | Decorating5(_, _, _, _, _, _, _, stopped, _)
  | Decorating6(_, _, _, _, _, _, _, _, stopped, _)
  | Decorating7(_, _, _, _, _, _, _, _, _, stopped, _) =>
    ! Atomic.exchange(stopped, true)
  | Disposed => false;

let completeWithResult = {
  let doComplete = (exn, subscriber) =>
    switch (subscriber) {
    | AutoDisposing(_, onComplete, _, disposable) =>
      try (onComplete(exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing1(ctx0, _, onComplete, _, disposable) =>
      try (onComplete(ctx0, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing2(ctx0, ctx1, _, onComplete, _, disposable) =>
      try (onComplete(ctx0, ctx1, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing3(ctx0, ctx1, ctx2, _, onComplete, _, disposable) =>
      try (onComplete(ctx0, ctx1, ctx2, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing4(ctx0, ctx1, ctx2, ctx3, _, onComplete, _, disposable) =>
      try (onComplete(ctx0, ctx1, ctx2, ctx3, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing5(
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        _,
        onComplete,
        _,
        disposable,
      ) =>
      try (onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing6(
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        _,
        onComplete,
        _,
        disposable,
      ) =>
      try (onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | AutoDisposing7(
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        _,
        onComplete,
        _,
        disposable,
      ) =>
      try (onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, exn)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      };
      disposable |> CompositeDisposable.dispose;
    | Decorating(_, onComplete, _, delegate) => onComplete(delegate, exn)
    | Decorating1(ctx0, _, onComplete, _, delegate) =>
      onComplete(ctx0, delegate, exn)
    | Decorating2(ctx0, ctx1, _, onComplete, _, delegate) =>
      onComplete(ctx0, ctx1, delegate, exn)
    | Decorating3(ctx0, ctx1, ctx2, _, onComplete, _, delegate) =>
      onComplete(ctx0, ctx1, ctx2, delegate, exn)
    | Decorating4(ctx0, ctx1, ctx2, ctx3, _, onComplete, _, delegate) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, delegate, exn)
    | Decorating5(ctx0, ctx1, ctx2, ctx3, ctx4, _, onComplete, _, delegate) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, delegate, exn)
    | Decorating6(
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        _,
        onComplete,
        _,
        delegate,
      ) =>
      onComplete(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, delegate, exn)
    | Decorating7(
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        _,
        onComplete,
        _,
        delegate,
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
    | AutoDisposing(onNext, _, _, disposable) =>
      try (onNext(next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing1(ctx0, onNext, _, _, disposable) =>
      try (onNext(ctx0, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing2(ctx0, ctx1, onNext, _, _, disposable) =>
      try (onNext(ctx0, ctx1, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing3(ctx0, ctx1, ctx2, onNext, _, _, disposable) =>
      try (onNext(ctx0, ctx1, ctx2, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing4(ctx0, ctx1, ctx2, ctx3, onNext, _, _, disposable) =>
      try (onNext(ctx0, ctx1, ctx2, ctx3, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing5(ctx0, ctx1, ctx2, ctx3, ctx4, onNext, _, _, disposable) =>
      try (onNext(ctx0, ctx1, ctx2, ctx3, ctx4, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing6(
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        onNext,
        _,
        _,
        disposable,
      ) =>
      try (onNext(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | AutoDisposing7(
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        onNext,
        _,
        _,
        disposable,
      ) =>
      try (onNext(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, next)) {
      | exn =>
        disposable |> CompositeDisposable.dispose;
        raise(exn);
      }
    | Decorating(onNext, _, _, delegate) => onNext(delegate, next)
    | Decorating1(ctx0, onNext, _, _, delegate) =>
      onNext(ctx0, delegate, next)
    | Decorating2(ctx0, ctx1, onNext, _, _, delegate) =>
      onNext(ctx0, ctx1, delegate, next)
    | Decorating3(ctx0, ctx1, ctx2, onNext, _, _, delegate) =>
      onNext(ctx0, ctx1, ctx2, delegate, next)
    | Decorating4(ctx0, ctx1, ctx2, ctx3, onNext, _, _, delegate) =>
      onNext(ctx0, ctx1, ctx2, ctx3, delegate, next)
    | Decorating5(ctx0, ctx1, ctx2, ctx3, ctx4, onNext, _, _, delegate) =>
      onNext(ctx0, ctx1, ctx2, ctx3, ctx4, delegate, next)
    | Decorating6(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, onNext, _, _, delegate) =>
      onNext(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, delegate, next)
    | Decorating7(
        ctx0,
        ctx1,
        ctx2,
        ctx3,
        ctx4,
        ctx5,
        ctx6,
        onNext,
        _,
        _,
        delegate,
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

let forwardOnNext5 = (_, _, _, _, _, subscriber, v) =>
  subscriber |> next(v);

let forwardOnNext6 = (_, _, _, _, _, _, subscriber, v) =>
  subscriber |> next(v);

let forwardOnNext7 = (_, _, _, _, _, _, _, subscriber, v) =>
  subscriber |> next(v);

let notify = (notif, subscriber) =>
  switch (notif) {
  | Notification.Next(v) => subscriber |> next(v)
  | Notification.Complete => subscriber |> complete
  | Notification.CompleteWithException(exn) => subscriber |> complete(~exn)
  };

let raiseIfDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.raiseIfDisposed;
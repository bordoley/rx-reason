type t('a) =
  | AutoDisposing(
      'a => unit,
      option(exn) => unit,
      ref(bool),
      CompositeDisposable.t,
    )
  | AutoDisposing1(
                    'ctx0,
                    ('ctx0, 'a) => unit,
                    ('ctx0, option(exn)) => unit,
                    ref(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | AutoDisposing2(
                    'ctx0,
                    'ctx1,
                    ('ctx0, 'ctx1, 'a) => unit,
                    ('ctx0, 'ctx1, option(exn)) => unit,
                    ref(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | AutoDisposing3(
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
                    ref(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | AutoDisposing4(
                    'ctx0,
                    'ctx1,
                    'ctx2,
                    'ctx3,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
                    ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
                    ref(bool),
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
                    ref(bool),
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
                    ref(bool),
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
                    ref(bool),
                    CompositeDisposable.t,
                  ): t('a)
  | Delegating(
                (t('b), 'a) => unit,
                (t('b), option(exn)) => unit,
                ref(bool),
                t('b),
              ): t('a)
  | Delegating1(
                 'ctx0,
                 ('ctx0, t('b), 'a) => unit,
                 ('ctx0, t('b), option(exn)) => unit,
                 ref(bool),
                 t('b),
               ): t('a)
  | Delegating2(
                 'ctx0,
                 'ctx1,
                 ('ctx0, 'ctx1, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, t('b), option(exn)) => unit,
                 ref(bool),
                 t('b),
               ): t('a)
  | Delegating3(
                 'ctx0,
                 'ctx1,
                 'ctx2,
                 ('ctx0, 'ctx1, 'ctx2, t('b), 'a) => unit,
                 ('ctx0, 'ctx1, 'ctx2, t('b), option(exn)) => unit,
                 ref(bool),
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
  | Delegating(_, _, _, delegate) => delegate |> asCompositeDisposable
  | Delegating1(_, _, _, _, delegate) => delegate |> asCompositeDisposable
  | Delegating2(_, _, _, _, _, delegate) => delegate |> asCompositeDisposable
  | Delegating3(_, _, _, _, _, _, delegate) =>
    delegate |> asCompositeDisposable
  | Disposed => CompositeDisposable.disposed;

let addDisposable = (disposable, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addDisposable(disposable)
  |> ignore;
  subscriber;
};

let addCompositeDisposable = (disposable, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addCompositeDisposable(disposable)
  |> ignore;
  subscriber;
};

let addSerialDisposable = (disposable, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addSerialDisposable(disposable)
  |> ignore;
  subscriber;
};

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
  let isStopped = ref(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Volatile.writeTrue, isStopped)
  |> ignore;
  AutoDisposing(onNext, onComplete, isStopped, disposable);
};

let createAutoDisposing1 = (~onNext, ~onComplete, ctx0) => {
  let isStopped = ref(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Volatile.writeTrue, isStopped)
  |> ignore;
  AutoDisposing1(ctx0, onNext, onComplete, isStopped, disposable);
};

let createAutoDisposing2 = (~onNext, ~onComplete, ctx0, ctx1) => {
  let isStopped = ref(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Volatile.writeTrue, isStopped)
  |> ignore;
  AutoDisposing2(ctx0, ctx1, onNext, onComplete, isStopped, disposable);
};

let createAutoDisposing3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2) => {
  let isStopped = ref(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Volatile.writeTrue, isStopped)
  |> ignore;
  AutoDisposing3(ctx0, ctx1, ctx2, onNext, onComplete, isStopped, disposable);
};

let createAutoDisposing4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3) => {
  let isStopped = ref(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Volatile.writeTrue, isStopped)
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
  let isStopped = ref(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Volatile.writeTrue, isStopped)
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
  let isStopped = ref(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Volatile.writeTrue, isStopped)
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
  let isStopped = ref(false);
  let disposable = CompositeDisposable.create();
  disposable
  |> CompositeDisposable.addTeardown1(Volatile.writeTrue, isStopped)
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

let delegate = (~onNext, ~onComplete, subscriber) => {
  let stopped = ref(false);
  subscriber |> addTeardown1(Volatile.writeTrue, stopped) |> ignore;
  Delegating(onNext, onComplete, stopped, subscriber);
};

let delegate1 = (~onNext, ~onComplete, ctx0, subscriber) => {
  let stopped = ref(false);
  subscriber |> addTeardown1(Volatile.writeTrue, stopped) |> ignore;
  Delegating1(ctx0, onNext, onComplete, stopped, subscriber);
};

let delegate2 = (~onNext, ~onComplete, ctx0, ctx1, subscriber) => {
  let stopped = ref(false);
  subscriber |> addTeardown1(Volatile.writeTrue, stopped) |> ignore;
  Delegating2(ctx0, ctx1, onNext, onComplete, stopped, subscriber);
};

let delegate3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, subscriber) => {
  let stopped = ref(false);
  subscriber |> addTeardown1(Volatile.writeTrue, stopped) |> ignore;
  Delegating3(ctx0, ctx1, ctx2, onNext, onComplete, stopped, subscriber);
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
  | Delegating(_, _, stopped, _)
  | Delegating1(_, _, _, stopped, _)
  | Delegating2(_, _, _, _, stopped, _)
  | Delegating3(_, _, _, _, _, stopped, _) => Volatile.read(stopped)
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
  | Delegating(_, _, stopped, _)
  | Delegating1(_, _, _, stopped, _)
  | Delegating2(_, _, _, _, stopped, _)
  | Delegating3(_, _, _, _, _, stopped, _) =>
    ! Interlocked.exchange(true, stopped)
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
    | Delegating(_, onComplete, _, delegate) => onComplete(delegate, exn)
    | Delegating1(ctx0, _, onComplete, _, delegate) =>
      onComplete(ctx0, delegate, exn)
    | Delegating2(ctx0, ctx1, _, onComplete, _, delegate) =>
      onComplete(ctx0, ctx1, delegate, exn)
    | Delegating3(ctx0, ctx1, ctx2, _, onComplete, _, delegate) =>
      onComplete(ctx0, ctx1, ctx2, delegate, exn)
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
    | Delegating(onNext, _, _, delegate) => onNext(delegate, next)
    | Delegating1(ctx0, onNext, _, _, delegate) =>
      onNext(ctx0, delegate, next)
    | Delegating2(ctx0, ctx1, onNext, _, _, delegate) =>
      onNext(ctx0, ctx1, delegate, next)
    | Delegating3(ctx0, ctx1, ctx2, onNext, _, _, delegate) =>
      onNext(ctx0, ctx1, ctx2, delegate, next)
    | Disposed => ()
    };

  (next, subscriber) => {
    let isStopped = subscriber |> isStopped;
    if (! isStopped) {
      doNext(next, subscriber);
    };
  };
};

let delegateOnComplete = (subscriber, exn) => subscriber |> complete(~exn?);

let delegateOnComplete1 = (_, subscriber, exn) =>
  subscriber |> complete(~exn?);

let delegateOnComplete2 = (_, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let delegateOnComplete3 = (_, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let delegateOnComplete4 = (_, _, _, _, subscriber, exn) =>
  subscriber |> complete(~exn?);

let delegateOnNext = (subscriber, v) => subscriber |> next(v);

let delegateOnNext1 = (_, subscriber, v) => subscriber |> next(v);

let delegateOnNext2 = (_, _, subscriber, v) => subscriber |> next(v);

let delegateOnNext3 = (_, _, _, subscriber, v) => subscriber |> next(v);

let delegateOnNext4 = (_, _, _, _, subscriber, v) => subscriber |> next(v);

let notify = (notif, subscriber) =>
  switch (notif) {
  | Notification.Next(v) => subscriber |> next(v)
  | Notification.Complete => subscriber |> complete
  | Notification.CompleteWithException(exn) => subscriber |> complete(~exn)
  };

let raiseIfDisposed = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.raiseIfDisposed;
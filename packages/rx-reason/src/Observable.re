type source('a) = Observer.t('a) => Disposable.t;

type t('a) =
  | Source(source('a)): t('a)
  | Lift1(source('b), Operator.t('b, 'a)): t('a)
  | Lift2(source('c), Operator.t('c, 'b), Operator.t('b, 'a)): t('a)
  | Lift3(
           source('d),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a)
  | Lift4(
           source('e),
           Operator.t('e, 'd),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  let asObservable: t('a) => observable('a);

  let publish:
    (
      ~onNext: 'a => unit=?,
      ~onComplete: option(exn) => unit=?,
      t('a),
      unit
    ) =>
    Disposable.t;

  let publishTo:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a), unit) =>
    Disposable.t;

  let subscribe:
    (~onNext: 'a => unit=?, ~onComplete: option(exn) => unit=?, t('a)) =>
    Disposable.t;

  let subscribeWith:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
    Disposable.t;
};

let asObservable = Functions.identity;

let subscribeSafe = (observer, source) =>
  try (source(observer)) {
  | exn =>
    let shouldRaise = observer |> Observer.completeWithResult(~exn) |> (!);
    if (shouldRaise) {
      /* This could happen when the onComplete is called synchronously in the
       * subscribe function which also throws.
       */
      raise(
        exn,
      );
    };
    Disposable.disposed;
  };

let subscribeObserver = (observer, observable) =>
  switch (observable) {
  | Source(source) => source |> subscribeSafe(observer)
  | Lift1(source, operator) =>
    let observer = operator(observer);
    source |> subscribeSafe(observer);
  | Lift2(source, op0, op1) =>
    let observer = op0(op1(observer));
    source |> subscribeSafe(observer);
  | Lift3(source, op0, op1, op2) =>
    let observer = op0(op1(op2(observer)));
    source |> subscribeSafe(observer);
  | Lift4(source, op0, op1, op2, op3) =>
    let observer = op0(op1(op2(op3(observer))));
    source |> subscribeSafe(observer);
  };

let subscribeWith = (~onNext, ~onComplete, observable) => {
  let subscription = SerialDisposable.create();

  let observer =
    Observer.createAutoDisposing(~onNext, ~onComplete, ~onDispose=() =>
      subscription |> SerialDisposable.dispose
    );

  subscription
  |> SerialDisposable.set(subscribeObserver(observer, observable));
  observer |> Observer.asDisposable;
};

let subscribe =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      observable,
    ) =>
  observable |> subscribeWith(~onNext, ~onComplete);

let createWithObserver = onSubscribe => Source(onSubscribe);

let create = onSubscribe =>
  Source(
    observer =>
      onSubscribe(
        ~onNext=Observer.forwardOnNext(observer),
        ~onComplete=Observer.forwardOnComplete(observer),
      ),
  );

let subscribeOn = (scheduler: Scheduler.t, observable: t('a)) : t('a) =>
  createWithObserver(observer =>
    scheduler(() => observable |> subscribeObserver(observer))
  );

let combineLatest2 =
    (~selector: ('a, 'b) => 'c, observable0: t('a), observable1: t('b))
    : t('c) =>
  create((~onNext, ~onComplete) => {
    let (v0, v1) = (MutableOption.create(), MutableOption.create());
    let lock = Lock.create();

    let tryOnNext =
      Functions.earlyReturnsUnit(() => {
        let haveValues =
          MutableOption.isNotEmpty(v0) && MutableOption.isNotEmpty(v1);
        if (haveValues) {
          let next =
            try (selector(MutableOption.get(v0), MutableOption.get(v1))) {
            | exn =>
              onComplete(Some(exn));
              Functions.returnUnit();
            };
          onNext(next);
        };
      });

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      tryOnNext();
      Lock.release(lock);
    };

    let doOnComplete = (other, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) => onComplete(exn)
      | None =>
        let shouldComplete = Disposable.isDisposed(other^);
        if (shouldComplete) {
          onComplete(None);
        };
      };
      Lock.release(lock);
    };

    let doSubscribe = (observable, v, s) =>
      observable
      |> subscribeWith(~onNext=doOnNext(v), ~onComplete=doOnComplete(s));

    let (s0, s1) = Disposable.(ref(disposed), ref(disposed));
    s0 := doSubscribe(observable0, v0, s1);
    s1 := doSubscribe(observable1, v1, s0);
    let (s0, s1) = (s0^, s1^);

    Disposable.create(() => {
      s0 |> Disposable.dispose;
      s1 |> Disposable.dispose;
      MutableOption.unset(v0);
      MutableOption.unset(v1);
    });
  });

let combineLatest3 =
    (
      ~selector: ('a, 'b, 'c) => 'd,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
    )
    : t('d) =>
  create((~onNext, ~onComplete) => {
    let (v0, v1, v2) = (
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
    );
    let lock = Lock.create();

    let tryOnNext =
      Functions.earlyReturnsUnit(() => {
        let haveValues =
          MutableOption.isNotEmpty(v0)
          && MutableOption.isNotEmpty(v1)
          && MutableOption.isNotEmpty(v2);
        if (haveValues) {
          let next =
            try (
              selector(
                MutableOption.get(v0),
                MutableOption.get(v1),
                MutableOption.get(v2),
              )
            ) {
            | exn =>
              onComplete(Some(exn));
              Functions.returnUnit();
            };
          onNext(next);
        };
      });

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      tryOnNext();
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) => onComplete(exn)
      | None =>
        let shouldComplete =
          Disposable.isDisposed(d0^) && Disposable.isDisposed(d1^);

        if (shouldComplete) {
          onComplete(None);
        };
      };
      Lock.release(lock);
    };

    let doSubscribe = (observable, v, s0, s1) =>
      observable
      |> subscribeWith(
           ~onNext=doOnNext(v),
           ~onComplete=doOnComplete(s0, s1),
         );

    let (s0, s1, s2) = (
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
    );

    s0 := doSubscribe(observable0, v0, s1, s2);
    s1 := doSubscribe(observable1, v1, s0, s2);
    s2 := doSubscribe(observable2, v2, s0, s1);

    let (s0, s1, s2) = (s0^, s1^, s2^);

    Disposable.create(() => {
      s0 |> Disposable.dispose;
      s1 |> Disposable.dispose;
      s2 |> Disposable.dispose;
      MutableOption.unset(v0);
      MutableOption.unset(v1);
      MutableOption.unset(v2);
    });
  });

let combineLatest4 =
    (
      ~selector: ('a, 'b, 'c, 'd) => 'e,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
      observable3: t('d),
    )
    : t('e) =>
  create((~onNext, ~onComplete) => {
    let (v0, v1, v2, v3) = (
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
    );
    let lock = Lock.create();

    let tryOnNext =
      Functions.earlyReturnsUnit(() => {
        let haveValues =
          MutableOption.isNotEmpty(v0)
          && MutableOption.isNotEmpty(v1)
          && MutableOption.isNotEmpty(v2)
          && MutableOption.isNotEmpty(v3);

        if (haveValues) {
          let next =
            try (
              selector(
                MutableOption.get(v0),
                MutableOption.get(v1),
                MutableOption.get(v2),
                MutableOption.get(v3),
              )
            ) {
            | exn =>
              onComplete(Some(exn));
              Functions.returnUnit();
            };
          onNext(next);
        };
      });

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      tryOnNext();
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, d2, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) => onComplete(exn)
      | None =>
        let shouldComplete =
          Disposable.isDisposed(d0^)
          && Disposable.isDisposed(d1^)
          && Disposable.isDisposed(d2^);

        if (shouldComplete) {
          onComplete(None);
        };
      };
      Lock.release(lock);
    };

    let doSubscribe = (observable, v, s0, s1, s2) =>
      observable
      |> subscribeWith(
           ~onNext=doOnNext(v),
           ~onComplete=doOnComplete(s0, s1, s2),
         );

    let (s0, s1, s2, s3) = (
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
    );

    s0 := doSubscribe(observable0, v0, s1, s2, s3);
    s1 := doSubscribe(observable1, v1, s0, s2, s3);
    s2 := doSubscribe(observable2, v2, s0, s1, s3);
    s3 := doSubscribe(observable3, v3, s0, s1, s2);

    let (s0, s1, s2, s3) = (s0^, s1^, s2^, s3^);

    Disposable.create(() => {
      s0 |> Disposable.dispose;
      s1 |> Disposable.dispose;
      s2 |> Disposable.dispose;
      s3 |> Disposable.dispose;
      MutableOption.unset(v0);
      MutableOption.unset(v1);
      MutableOption.unset(v2);
      MutableOption.unset(v3);
    });
  });

let combineLatest5 =
    (
      ~selector: ('a, 'b, 'c, 'd, 'e) => 'f,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
      observable3: t('d),
      observable4: t('e),
    )
    : t('f) =>
  create((~onNext, ~onComplete) => {
    let (v0, v1, v2, v3, v4) = (
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
    );
    let lock = Lock.create();

    let tryOnNext =
      Functions.earlyReturnsUnit(() => {
        let haveValues =
          MutableOption.isNotEmpty(v0)
          && MutableOption.isNotEmpty(v1)
          && MutableOption.isNotEmpty(v2)
          && MutableOption.isNotEmpty(v3)
          && MutableOption.isNotEmpty(v4);

        if (haveValues) {
          let next =
            try (
              selector(
                MutableOption.get(v0),
                MutableOption.get(v1),
                MutableOption.get(v2),
                MutableOption.get(v3),
                MutableOption.get(v4),
              )
            ) {
            | exn =>
              onComplete(Some(exn));
              Functions.returnUnit();
            };
          onNext(next);
        };
      });

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      tryOnNext();
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, d2, d3, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) => onComplete(exn)
      | None =>
        let shouldComplete =
          Disposable.isDisposed(d0^)
          && Disposable.isDisposed(d1^)
          && Disposable.isDisposed(d2^)
          && Disposable.isDisposed(d3^);

        if (shouldComplete) {
          onComplete(None);
        };
      };
      Lock.release(lock);
    };

    let doSubscribe = (observable, v, s0, s1, s2, s3) =>
      observable
      |> subscribeWith(
           ~onNext=doOnNext(v),
           ~onComplete=doOnComplete(s0, s1, s2, s3),
         );

    let (s0, s1, s2, s3, s4) = (
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
    );

    s0 := doSubscribe(observable0, v0, s1, s2, s3, s4);
    s1 := doSubscribe(observable1, v1, s0, s2, s3, s4);
    s2 := doSubscribe(observable2, v2, s0, s1, s3, s4);
    s3 := doSubscribe(observable3, v3, s0, s1, s2, s4);
    s4 := doSubscribe(observable4, v4, s0, s1, s2, s3);

    let (s0, s1, s2, s3, s4) = (s0^, s1^, s2^, s3^, s4^);

    Disposable.create(() => {
      s0 |> Disposable.dispose;
      s1 |> Disposable.dispose;
      s2 |> Disposable.dispose;
      s3 |> Disposable.dispose;
      s4 |> Disposable.dispose;
      MutableOption.unset(v0);
      MutableOption.unset(v1);
      MutableOption.unset(v2);
      MutableOption.unset(v3);
      MutableOption.unset(v4);
    });
  });

let combineLatest6 =
    (
      ~selector: ('a, 'b, 'c, 'd, 'e, 'f) => 'g,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
      observable3: t('d),
      observable4: t('e),
      observable5: t('f),
    )
    : t('g) =>
  create((~onNext, ~onComplete) => {
    let (v0, v1, v2, v3, v4, v5) = (
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
    );
    let lock = Lock.create();

    let tryOnNext =
      Functions.earlyReturnsUnit(() => {
        let haveValues =
          MutableOption.isNotEmpty(v0)
          && MutableOption.isNotEmpty(v1)
          && MutableOption.isNotEmpty(v2)
          && MutableOption.isNotEmpty(v3)
          && MutableOption.isNotEmpty(v4)
          && MutableOption.isNotEmpty(v5);

        if (haveValues) {
          let next =
            try (
              selector(
                MutableOption.get(v0),
                MutableOption.get(v1),
                MutableOption.get(v2),
                MutableOption.get(v3),
                MutableOption.get(v4),
                MutableOption.get(v5),
              )
            ) {
            | exn =>
              onComplete(Some(exn));
              Functions.returnUnit();
            };
          onNext(next);
        };
      });

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      tryOnNext();
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, d2, d3, d4, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) => onComplete(exn)
      | None =>
        let shouldComplete =
          Disposable.isDisposed(d0^)
          && Disposable.isDisposed(d1^)
          && Disposable.isDisposed(d2^)
          && Disposable.isDisposed(d3^)
          && Disposable.isDisposed(d4^);

        if (shouldComplete) {
          onComplete(None);
        };
      };
      Lock.release(lock);
    };

    let doSubscribe = (observable, v, s0, s1, s2, s3, s4) =>
      observable
      |> subscribeWith(
           ~onNext=doOnNext(v),
           ~onComplete=doOnComplete(s0, s1, s2, s3, s4),
         );

    let (s0, s1, s2, s3, s4, s5) = (
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
    );

    s0 := doSubscribe(observable0, v0, s1, s2, s3, s4, s5);
    s1 := doSubscribe(observable1, v1, s0, s2, s3, s4, s5);
    s2 := doSubscribe(observable2, v2, s0, s1, s3, s4, s5);
    s3 := doSubscribe(observable3, v3, s0, s1, s2, s4, s5);
    s4 := doSubscribe(observable4, v4, s0, s1, s2, s3, s5);
    s5 := doSubscribe(observable5, v5, s0, s1, s2, s3, s4);

    let (s0, s1, s2, s3, s4, s5) = (s0^, s1^, s2^, s3^, s4^, s5^);

    Disposable.create(() => {
      s0 |> Disposable.dispose;
      s1 |> Disposable.dispose;
      s2 |> Disposable.dispose;
      s3 |> Disposable.dispose;
      s4 |> Disposable.dispose;
      s5 |> Disposable.dispose;
      MutableOption.unset(v0);
      MutableOption.unset(v1);
      MutableOption.unset(v2);
      MutableOption.unset(v3);
      MutableOption.unset(v4);
      MutableOption.unset(v5);
    });
  });

let combineLatest7 =
    (
      ~selector: ('a, 'b, 'c, 'd, 'e, 'f, 'g) => 'h,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
      observable3: t('d),
      observable4: t('e),
      observable5: t('f),
      observable6: t('g),
    )
    : t('h) =>
  create((~onNext, ~onComplete) => {
    let (v0, v1, v2, v3, v4, v5, v6) = (
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
    );
    let lock = Lock.create();

    let tryOnNext =
      Functions.earlyReturnsUnit(() => {
        let haveValues =
          MutableOption.isNotEmpty(v0)
          && MutableOption.isNotEmpty(v1)
          && MutableOption.isNotEmpty(v2)
          && MutableOption.isNotEmpty(v3)
          && MutableOption.isNotEmpty(v4)
          && MutableOption.isNotEmpty(v5)
          && MutableOption.isNotEmpty(v6);

        if (haveValues) {
          let next =
            try (
              selector(
                MutableOption.get(v0),
                MutableOption.get(v1),
                MutableOption.get(v2),
                MutableOption.get(v3),
                MutableOption.get(v4),
                MutableOption.get(v5),
                MutableOption.get(v6),
              )
            ) {
            | exn =>
              onComplete(Some(exn));
              Functions.returnUnit();
            };
          onNext(next);
        };
      });

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      tryOnNext();
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, d2, d3, d4, d5, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) => onComplete(exn)
      | None =>
        let shouldComplete =
          Disposable.isDisposed(d0^)
          && Disposable.isDisposed(d1^)
          && Disposable.isDisposed(d2^)
          && Disposable.isDisposed(d3^)
          && Disposable.isDisposed(d4^)
          && Disposable.isDisposed(d5^);

        if (shouldComplete) {
          onComplete(None);
        };
      };
      Lock.release(lock);
    };

    let doSubscribe = (observable, v, s0, s1, s2, s3, s4, s5) =>
      observable
      |> subscribeWith(
           ~onNext=doOnNext(v),
           ~onComplete=doOnComplete(s0, s1, s2, s3, s4, s5),
         );

    let (s0, s1, s2, s3, s4, s5, s6) = (
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
      ref(Disposable.disposed),
    );

    s0 := doSubscribe(observable0, v0, s1, s2, s3, s4, s5, s6);
    s1 := doSubscribe(observable1, v1, s0, s2, s3, s4, s5, s6);
    s2 := doSubscribe(observable2, v2, s0, s1, s3, s4, s5, s6);
    s3 := doSubscribe(observable3, v3, s0, s1, s2, s4, s5, s6);
    s4 := doSubscribe(observable4, v4, s0, s1, s2, s3, s5, s6);
    s5 := doSubscribe(observable5, v5, s0, s1, s2, s3, s4, s6);
    s6 := doSubscribe(observable6, v6, s0, s1, s2, s3, s4, s5);

    let (s0, s1, s2, s3, s4, s5, s6) = (s0^, s1^, s2^, s3^, s4^, s5^, s6^);

    Disposable.create(() => {
      s0 |> Disposable.dispose;
      s1 |> Disposable.dispose;
      s2 |> Disposable.dispose;
      s3 |> Disposable.dispose;
      s4 |> Disposable.dispose;
      s5 |> Disposable.dispose;
      s6 |> Disposable.dispose;
      MutableOption.unset(v0);
      MutableOption.unset(v1);
      MutableOption.unset(v2);
      MutableOption.unset(v3);
      MutableOption.unset(v4);
      MutableOption.unset(v5);
      MutableOption.unset(v6);
    });
  });

let concat =
    (~scheduler=Scheduler.immediate, observables: list(t('a)))
    : t('a) =>
  create((~onNext, ~onComplete) => {
    let subscription = SerialDisposable.create();

    let rec scheduleSubscription = observables => {
      let newSubscription =
        switch (observables) {
        | [hd, ...tail] =>
          scheduler(() =>
            hd
            |> subscribeWith(~onNext, ~onComplete=exn =>
                 switch (exn) {
                 | Some(_) => onComplete(exn)
                 | None =>
                   /* Cancel the current subscription here */
                   subscription |> SerialDisposable.set(Disposable.disposed);
                   scheduleSubscription(tail);
                 }
               )
          )
        | [] =>
          onComplete(None);
          Disposable.disposed;
        };

      /* An observable may complete synchronously and continue with an
       * async observable. Avoid canceling the async observable in that case.
       */
      if (! Disposable.isDisposed(newSubscription)) {
        subscription |> SerialDisposable.set(newSubscription);
      };
    };

    scheduleSubscription(observables);
    subscription |> SerialDisposable.asDisposable;
  });

let defer = (f: unit => t('a)) : t('a) =>
  createWithObserver(observer => f() |> subscribeObserver(observer));

let empty = (~scheduler=Scheduler.immediate, ()) =>
  scheduler === Scheduler.immediate ?
    create((~onNext as _, ~onComplete) => {
      onComplete(None);
      Disposable.disposed;
    }) :
    create((~onNext as _, ~onComplete) =>
      scheduler(() => {
        onComplete(None);
        Disposable.disposed;
      })
    );

let lift = (operator: Operator.t('a, 'b), observable: t('a)) : t('b) =>
  switch (observable) {
  | Source(source) => Lift1(source, operator)
  | Lift1(source, op0) => Lift2(source, op0, operator)
  | Lift2(source, op0, op1) => Lift3(source, op0, op1, operator)
  | Lift3(source, op0, op1, op2) => Lift4(source, op0, op1, op2, operator)
  | Lift4(source, op0, op1, op2, op3) =>
      let operator = observer => op0 @@ op1 @@ op2 @@ op3 @@ operator @@ observer;
      Lift1(source, operator)
  };

let merge = (observables: list(t('a))) : t('a) => {
  let count = observables |> List.length;

  create((~onNext, ~onComplete) => {
    let activeCount = ref(count);
    let lock = Lock.create();
    observables
    |> List.map(
         subscribeWith(
           ~onNext=
             next => {
               Lock.acquire(lock);
               onNext(next);
               Lock.release(lock);
             },
           ~onComplete=
             exn => {
               Lock.acquire(lock);
               let shouldComplete =
                 switch (exn) {
                 | Some(_) => true
                 | None =>
                   let oldActiveCount = Interlocked.decrement(activeCount);
                   oldActiveCount <= 0;
                 };
               if (shouldComplete) {
                 onComplete(exn);
               };
               Lock.release(lock);
             },
         ),
       )
    |> Disposable.compose;
  });
};

let never = Source(_ => Disposable.empty());

let ofAbsoluteTimeNotifications =
    (
      ~scheduler as {now, scheduleWithDelay}: ClockScheduler.t,
      notifications: list((float, Notification.t('a))),
    )
    : t('a) =>
  createWithObserver(observer => {
    let rec loop = lst =>
      switch (lst) {
      | [(time, notif), ...tail] =>
        let delay = time -. now();
        if (delay > 0.0) {
          scheduleWithDelay(
            delay,
            () => {
              observer |> Observer.notify(notif);
              loop(tail);
            },
          );
        } else {
          loop(tail);
        };
      | [] => Disposable.disposed
      };

    loop(notifications);
  });

let ofList = (~scheduler=Scheduler.immediate, list: list('a)) : t('a) =>
  scheduler === Scheduler.immediate ?
    create((~onNext, ~onComplete) => {
      let rec loop = list =>
        switch (list) {
        | [hd, ...tail] =>
          onNext(hd);
          loop(tail);
        | [] => onComplete(None)
        };
      loop(list);
      Disposable.disposed;
    }) :
    create((~onNext, ~onComplete) => {
      let rec loop = (list, ()) =>
        switch (list) {
        | [hd] =>
          onNext(hd);
          onComplete(None);
          Disposable.disposed;
        | [hd, ...tail] =>
          onNext(hd);
          scheduler(loop(tail));
        | [] =>
          onComplete(None);
          Disposable.disposed;
        };
      scheduler(loop(list));
    });

let ofNotifications =
    (
      ~scheduler as schedule=Scheduler.immediate,
      notifications: list(Notification.t('a)),
    )
    : t('a) =>
  createWithObserver(
    schedule === Scheduler.immediate ?
      observer => {
        let rec loop = list =>
          switch (list) {
          | [hd, ...tail] =>
            observer |> Observer.notify(hd);
            loop(tail);
          | [] => ()
          };
        loop(notifications);
        Disposable.disposed;
      } :
      (
        observer => {
          let rec loop = (list, ()) =>
            switch (list) {
            | [hd] =>
              observer |> Observer.notify(hd);
              Disposable.disposed;
            | [hd, ...tail] =>
              observer |> Observer.notify(hd);
              schedule(loop(tail));
            | [] => Disposable.disposed
            };
          schedule(loop(notifications));
        }
      ),
  );

let ofRelativeTimeNotifications =
    (
      ~scheduler as schedule: DelayScheduler.t,
      notifications: list((float, Notification.t('a))),
    )
    : t('a) =>
  createWithObserver(observer => {
    let rec loop = (lst, previousDelay) =>
      switch (lst) {
      | [(delay, notif), ...tail] =>
        schedule(
          max(0.0, delay -. previousDelay),
          () => {
            observer |> Observer.notify(notif);
            loop(tail, delay);
          },
        )
      | [] => Disposable.disposed
      };

    loop(notifications, 0.0);
  });

let ofValue = (~scheduler=Scheduler.immediate, value: 'a) : t('a) =>
  scheduler === Scheduler.immediate ?
    create((~onNext, ~onComplete) => {
      onNext(value);
      onComplete(None);
      Disposable.disposed;
    }) :
    create((~onNext, ~onComplete) =>
      scheduler(() => {
        onNext(value);
        scheduler(() => {
          onComplete(None);
          Disposable.disposed;
        });
      })
    );

let onSubscribeOperator = (f, observer) => {
  let callbackDisposable = f();
  Observer.create(
    ~onNext=Observer.forwardOnNext(observer),
    ~onComplete=Observer.forwardOnComplete(observer),
    ~onDispose=() => {
      callbackDisposable |> Disposable.dispose;
      observer |> Observer.dispose;
    },
  );
};

let onSubscribe = (f, observable) =>
  observable |> lift(onSubscribeOperator(f));

let raise = (~scheduler=Scheduler.immediate, exn: exn) : t('a) => {
  let exn = Some(exn);

  scheduler === Scheduler.immediate ?
    create((~onNext as _, ~onComplete) => {
      onComplete(exn);
      Disposable.disposed;
    }) :
    create((~onNext as _, ~onComplete) =>
      scheduler(() => {
        onComplete(exn);
        Disposable.disposed;
      })
    );
};

let publishTo = (~onNext, ~onComplete, observable) => {
  let connection = ref(Disposable.disposed);
  let active = ref(false);

  () => {
    if (! Interlocked.exchange(true, active)) {
      let subscription = observable |> subscribeWith(~onNext, ~onComplete);
      let newConnection =
        Disposable.create(() => {
          subscription |> Disposable.dispose;
          Volatile.write(false, active);
        });

      Volatile.write(newConnection, connection);
    };
    Volatile.read(connection);
  };
};

let publish =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      observable,
    ) =>
  publishTo(~onNext, ~onComplete, observable);

let repeatOperator = (shouldRetry, observable, observer) => {
  let subscription = SerialDisposable.create();
  let setupSubscription = ref(Functions.alwaysUnit);

  let onComplete =
    Functions.earlyReturnsUnit1(exn => {
      let shouldComplete =
        try (! shouldRetry(exn)) {
        | exn =>
          observer |> Observer.complete(~exn);
          Functions.returnUnit();
        };

      shouldComplete ?
        observer |> Observer.complete(~exn?) : setupSubscription^();
    });

  setupSubscription :=
    (
      () => {
        let alreadyDisposed = subscription |> SerialDisposable.isDisposed;

        if (! alreadyDisposed) {
          subscription |> SerialDisposable.get |> Disposable.dispose;
          let newInnerSubscription =
            observable
            |> subscribe(
                 ~onNext=Observer.forwardOnNext(observer),
                 ~onComplete,
               );
          subscription |> SerialDisposable.set(newInnerSubscription);
        };
      }
    );

  Observer.create(
    ~onNext=Observer.forwardOnNext(observer),
    ~onComplete,
    ~onDispose=() => {
      subscription |> SerialDisposable.dispose;
      observer |> Observer.dispose;
    },
  );
};

let repeatInternal = (shouldRetry, observable) =>
  observable |> lift(repeatOperator(shouldRetry, observable));

let repeat = (~predicate=Functions.alwaysTrue, observable) =>
  observable
  |> repeatInternal(exn =>
       switch (exn) {
       | None => predicate()
       | Some(_) => false
       }
     );

let retry = (~predicate=Functions.alwaysTrue, observable) =>
  observable
  |> repeatInternal(exn =>
       switch (exn) {
       | None => false
       | Some(exn) => predicate(exn)
       }
     );

let startWithList =
    (~scheduler=Scheduler.immediate, values: list('a), observable: t('a)) =>
  concat([ofList(~scheduler, values), observable]);

let startWithValue =
    (~scheduler=Scheduler.immediate, value: 'a, observable: t('a)) =>
  concat([ofValue(~scheduler, value), observable]);
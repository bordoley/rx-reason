type t('a) =
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) => Disposable.t;

let subscribe = (~onNext, ~onComplete, observable: t('a)) : Disposable.t =>
  observable(~onNext, ~onComplete);

let subscribeObserver =
    (observer: Observer.t('a), observable: t('a))
    : Disposable.t =>
  subscribe(
    ~onNext=next => observer |> Observer.next(next),
    ~onComplete=exn => observer |> Observer.complete(exn),
    observable,
  );

let createWithObserver =
    (onSubscribe: Observer.t('a) => Disposable.t)
    : t('a) =>
  (~onNext, ~onComplete) => {
    let subscription = ref(Disposable.disposed);
    let observer =
      Observer.create(~onNext, ~onComplete, ~onDispose=() =>
        Interlocked.exchange(Disposable.disposed, subscription)
        |> Disposable.dispose
      );
    subscription :=
      (
        try (onSubscribe(observer)) {
        | exn =>
          let shouldRaise =
            observer |> Observer.completeWithResult(Some(exn)) |> (!);
          if (shouldRaise) {
            /* This could happen when the onComplete is called synchronously in the
             * subscribe function which also throws.
             */
            raise(
              exn,
            );
          };
          Disposable.disposed;
        }
      );
    observer |> Observer.toDisposable;
  };

let create = onSubscribe : t('a) =>
  createWithObserver(observer => subscribeObserver(observer, onSubscribe));

let subscribeOn = (scheduler: Scheduler.t, observable: t('a)) : t('a) =>
  create((~onNext, ~onComplete) =>
    scheduler(() => observable |> subscribe(~onNext, ~onComplete))
  );

let combineLatest2 =
    (~selector: ('a, 'b) => 'c, observable0: t('a), observable1: t('b))
    : t('c) =>
  create((~onNext, ~onComplete) => {
    let v0 = MutableOption.empty();
    let v1 = MutableOption.empty();

    let s0 = AssignableDisposable.create();
    let s1 = AssignableDisposable.create();
    let subscription = AssignableDisposable.create();

    let lock = Lock.create();

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);

      let haveValues =
        v0 |> MutableOption.isNotEmpty && v1 |> MutableOption.isNotEmpty;
      if (haveValues) {
        onNext(
          selector(
            v0 |> MutableOption.firstOrRaise,
            v1 |> MutableOption.firstOrRaise,
          ),
        );
      };
      Lock.release(lock);
    };

    let doOnComplete = (other, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) =>
        onComplete(exn);
        subscription |> AssignableDisposable.dispose;
      | None =>
        let shouldComplete = other |> AssignableDisposable.isDisposed;
        if (shouldComplete) {
          onComplete(None);
          subscription |> AssignableDisposable.dispose;
        };
      };
      Lock.release(lock);
    };

    s0
    |> AssignableDisposable.set(
         observable0
         |> subscribe(~onNext=doOnNext(v0), ~onComplete=doOnComplete(s1)),
       );
    s1
    |> AssignableDisposable.set(
         observable1
         |> subscribe(~onNext=doOnNext(v1), ~onComplete=doOnComplete(s0)),
       );

    subscription
    |> AssignableDisposable.set(
         Disposable.create(() => {
           s0 |> AssignableDisposable.dispose;
           s1 |> AssignableDisposable.dispose;
           MutableOption.unset(v0);
           MutableOption.unset(v1);
         }),
       );
    subscription |> AssignableDisposable.toDisposable;
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
    let v0 = MutableOption.empty();
    let v1 = MutableOption.empty();
    let v2 = MutableOption.empty();

    let s0 = AssignableDisposable.create();
    let s1 = AssignableDisposable.create();
    let s2 = AssignableDisposable.create();
    let subscription = AssignableDisposable.create();

    let lock = Lock.create();

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);

      let haveValues =
        v0
        |> MutableOption.isNotEmpty
        && v1
        |> MutableOption.isNotEmpty
        && v2
        |> MutableOption.isNotEmpty;
      if (haveValues) {
        onNext(
          selector(
            v0 |> MutableOption.firstOrRaise,
            v1 |> MutableOption.firstOrRaise,
            v2 |> MutableOption.firstOrRaise,
          ),
        );
      };
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) =>
        onComplete(exn);
        subscription |> AssignableDisposable.dispose;
      | None =>
        let shouldComplete =
          d0
          |> AssignableDisposable.isDisposed
          && d1
          |> AssignableDisposable.isDisposed;

        if (shouldComplete) {
          onComplete(None);
          subscription |> AssignableDisposable.dispose;
        };
      };
      Lock.release(lock);
    };

    s0
    |> AssignableDisposable.set(
         observable0
         |> subscribe(
              ~onNext=doOnNext(v0),
              ~onComplete=doOnComplete(s1, s2),
            ),
       );
    s1
    |> AssignableDisposable.set(
         observable1
         |> subscribe(
              ~onNext=doOnNext(v1),
              ~onComplete=doOnComplete(s0, s2),
            ),
       );
    s2
    |> AssignableDisposable.set(
         observable2
         |> subscribe(
              ~onNext=doOnNext(v2),
              ~onComplete=doOnComplete(s0, s1),
            ),
       );

    subscription
    |> AssignableDisposable.set(
         Disposable.create(() => {
           s0 |> AssignableDisposable.dispose;
           s1 |> AssignableDisposable.dispose;
           s2 |> AssignableDisposable.dispose;
           MutableOption.unset(v0);
           MutableOption.unset(v1);
           MutableOption.unset(v2);
         }),
       );
    subscription |> AssignableDisposable.toDisposable;
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
    let v0 = MutableOption.empty();
    let v1 = MutableOption.empty();
    let v2 = MutableOption.empty();
    let v3 = MutableOption.empty();

    let s0 = AssignableDisposable.create();
    let s1 = AssignableDisposable.create();
    let s2 = AssignableDisposable.create();
    let s3 = AssignableDisposable.create();
    let subscription = AssignableDisposable.create();

    let lock = Lock.create();

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);

      let haveValues =
        v0
        |> MutableOption.isNotEmpty
        && v1
        |> MutableOption.isNotEmpty
        && v2
        |> MutableOption.isNotEmpty
        && v3
        |> MutableOption.isNotEmpty;
      if (haveValues) {
        onNext(
          selector(
            v0 |> MutableOption.firstOrRaise,
            v1 |> MutableOption.firstOrRaise,
            v2 |> MutableOption.firstOrRaise,
            v3 |> MutableOption.firstOrRaise,
          ),
        );
      };
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, d2, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) =>
        onComplete(exn);
        subscription |> AssignableDisposable.dispose;
      | None =>
        let shouldComplete =
          d0
          |> AssignableDisposable.isDisposed
          && d1
          |> AssignableDisposable.isDisposed
          && d2
          |> AssignableDisposable.isDisposed;

        if (shouldComplete) {
          onComplete(None);
          subscription |> AssignableDisposable.dispose;
        };
      };
      Lock.release(lock);
    };

    s0
    |> AssignableDisposable.set(
         observable0
         |> subscribe(
              ~onNext=doOnNext(v0),
              ~onComplete=doOnComplete(s1, s2, s3),
            ),
       );
    s1
    |> AssignableDisposable.set(
         observable1
         |> subscribe(
              ~onNext=doOnNext(v1),
              ~onComplete=doOnComplete(s0, s2, s3),
            ),
       );
    s2
    |> AssignableDisposable.set(
         observable2
         |> subscribe(
              ~onNext=doOnNext(v2),
              ~onComplete=doOnComplete(s0, s1, s3),
            ),
       );
    s3
    |> AssignableDisposable.set(
         observable3
         |> subscribe(
              ~onNext=doOnNext(v3),
              ~onComplete=doOnComplete(s0, s1, s2),
            ),
       );

    subscription
    |> AssignableDisposable.set(
         Disposable.create(() => {
           s0 |> AssignableDisposable.dispose;
           s1 |> AssignableDisposable.dispose;
           s2 |> AssignableDisposable.dispose;
           s3 |> AssignableDisposable.dispose;
           MutableOption.unset(v0);
           MutableOption.unset(v1);
           MutableOption.unset(v2);
           MutableOption.unset(v3);
         }),
       );
    subscription |> AssignableDisposable.toDisposable;
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
    let v0 = MutableOption.empty();
    let v1 = MutableOption.empty();
    let v2 = MutableOption.empty();
    let v3 = MutableOption.empty();
    let v4 = MutableOption.empty();

    let s0 = AssignableDisposable.create();
    let s1 = AssignableDisposable.create();
    let s2 = AssignableDisposable.create();
    let s3 = AssignableDisposable.create();
    let s4 = AssignableDisposable.create();
    let subscription = AssignableDisposable.create();

    let lock = Lock.create();

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);

      let haveValues =
        v0
        |> MutableOption.isNotEmpty
        && v1
        |> MutableOption.isNotEmpty
        && v2
        |> MutableOption.isNotEmpty
        && v3
        |> MutableOption.isNotEmpty
        && v4
        |> MutableOption.isNotEmpty;
      if (haveValues) {
        onNext(
          selector(
            v0 |> MutableOption.firstOrRaise,
            v1 |> MutableOption.firstOrRaise,
            v2 |> MutableOption.firstOrRaise,
            v3 |> MutableOption.firstOrRaise,
            v4 |> MutableOption.firstOrRaise,
          ),
        );
      };
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, d2, d3, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) =>
        onComplete(exn);
        subscription |> AssignableDisposable.dispose;
      | None =>
        let shouldComplete =
          d0
          |> AssignableDisposable.isDisposed
          && d1
          |> AssignableDisposable.isDisposed
          && d2
          |> AssignableDisposable.isDisposed
          && d3
          |> AssignableDisposable.isDisposed;

        if (shouldComplete) {
          onComplete(None);
          subscription |> AssignableDisposable.dispose;
        };
      };
      Lock.release(lock);
    };

    s0
    |> AssignableDisposable.set(
         observable0
         |> subscribe(
              ~onNext=doOnNext(v0),
              ~onComplete=doOnComplete(s1, s2, s3, s4),
            ),
       );
    s1
    |> AssignableDisposable.set(
         observable1
         |> subscribe(
              ~onNext=doOnNext(v1),
              ~onComplete=doOnComplete(s0, s2, s3, s4),
            ),
       );
    s2
    |> AssignableDisposable.set(
         observable2
         |> subscribe(
              ~onNext=doOnNext(v2),
              ~onComplete=doOnComplete(s0, s1, s3, s4),
            ),
       );
    s3
    |> AssignableDisposable.set(
         observable3
         |> subscribe(
              ~onNext=doOnNext(v3),
              ~onComplete=doOnComplete(s0, s1, s2, s4),
            ),
       );
    s4
    |> AssignableDisposable.set(
         observable4
         |> subscribe(
              ~onNext=doOnNext(v4),
              ~onComplete=doOnComplete(s0, s1, s2, s3),
            ),
       );

    subscription
    |> AssignableDisposable.set(
         Disposable.create(() => {
           s0 |> AssignableDisposable.dispose;
           s1 |> AssignableDisposable.dispose;
           s2 |> AssignableDisposable.dispose;
           s3 |> AssignableDisposable.dispose;
           s4 |> AssignableDisposable.dispose;
           MutableOption.unset(v0);
           MutableOption.unset(v1);
           MutableOption.unset(v2);
           MutableOption.unset(v3);
           MutableOption.unset(v4);
         }),
       );
    subscription |> AssignableDisposable.toDisposable;
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
    let v0 = MutableOption.empty();
    let v1 = MutableOption.empty();
    let v2 = MutableOption.empty();
    let v3 = MutableOption.empty();
    let v4 = MutableOption.empty();
    let v5 = MutableOption.empty();

    let s0 = AssignableDisposable.create();
    let s1 = AssignableDisposable.create();
    let s2 = AssignableDisposable.create();
    let s3 = AssignableDisposable.create();
    let s4 = AssignableDisposable.create();
    let s5 = AssignableDisposable.create();
    let subscription = AssignableDisposable.create();

    let lock = Lock.create();

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);

      let haveValues =
        v0
        |> MutableOption.isNotEmpty
        && v1
        |> MutableOption.isNotEmpty
        && v2
        |> MutableOption.isNotEmpty
        && v3
        |> MutableOption.isNotEmpty
        && v4
        |> MutableOption.isNotEmpty
        && v5
        |> MutableOption.isNotEmpty;
      if (haveValues) {
        onNext(
          selector(
            v0 |> MutableOption.firstOrRaise,
            v1 |> MutableOption.firstOrRaise,
            v2 |> MutableOption.firstOrRaise,
            v3 |> MutableOption.firstOrRaise,
            v4 |> MutableOption.firstOrRaise,
            v5 |> MutableOption.firstOrRaise,
          ),
        );
      };
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, d2, d3, d4, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) =>
        onComplete(exn);
        subscription |> AssignableDisposable.dispose;
      | None =>
        let shouldComplete =
          d0
          |> AssignableDisposable.isDisposed
          && d1
          |> AssignableDisposable.isDisposed
          && d2
          |> AssignableDisposable.isDisposed
          && d3
          |> AssignableDisposable.isDisposed
          && d4
          |> AssignableDisposable.isDisposed;

        if (shouldComplete) {
          onComplete(None);
          subscription |> AssignableDisposable.dispose;
        };
      };
      Lock.release(lock);
    };

    s0
    |> AssignableDisposable.set(
         observable0
         |> subscribe(
              ~onNext=doOnNext(v0),
              ~onComplete=doOnComplete(s1, s2, s3, s4, s5),
            ),
       );
    s1
    |> AssignableDisposable.set(
         observable1
         |> subscribe(
              ~onNext=doOnNext(v1),
              ~onComplete=doOnComplete(s0, s2, s3, s4, s5),
            ),
       );
    s2
    |> AssignableDisposable.set(
         observable2
         |> subscribe(
              ~onNext=doOnNext(v2),
              ~onComplete=doOnComplete(s0, s1, s3, s4, s5),
            ),
       );
    s3
    |> AssignableDisposable.set(
         observable3
         |> subscribe(
              ~onNext=doOnNext(v3),
              ~onComplete=doOnComplete(s0, s1, s2, s4, s5),
            ),
       );
    s4
    |> AssignableDisposable.set(
         observable4
         |> subscribe(
              ~onNext=doOnNext(v4),
              ~onComplete=doOnComplete(s0, s1, s2, s3, s5),
            ),
       );
    s5
    |> AssignableDisposable.set(
         observable5
         |> subscribe(
              ~onNext=doOnNext(v5),
              ~onComplete=doOnComplete(s0, s1, s2, s3, s4),
            ),
       );

    subscription
    |> AssignableDisposable.set(
         Disposable.create(() => {
           s0 |> AssignableDisposable.dispose;
           s1 |> AssignableDisposable.dispose;
           s2 |> AssignableDisposable.dispose;
           s3 |> AssignableDisposable.dispose;
           s4 |> AssignableDisposable.dispose;
           s5 |> AssignableDisposable.dispose;
           MutableOption.unset(v0);
           MutableOption.unset(v1);
           MutableOption.unset(v2);
           MutableOption.unset(v3);
           MutableOption.unset(v4);
           MutableOption.unset(v5);
         }),
       );
    subscription |> AssignableDisposable.toDisposable;
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
    let v0 = MutableOption.empty();
    let v1 = MutableOption.empty();
    let v2 = MutableOption.empty();
    let v3 = MutableOption.empty();
    let v4 = MutableOption.empty();
    let v5 = MutableOption.empty();
    let v6 = MutableOption.empty();

    let s0 = AssignableDisposable.create();
    let s1 = AssignableDisposable.create();
    let s2 = AssignableDisposable.create();
    let s3 = AssignableDisposable.create();
    let s4 = AssignableDisposable.create();
    let s5 = AssignableDisposable.create();
    let s6 = AssignableDisposable.create();
    let subscription = AssignableDisposable.create();

    let lock = Lock.create();

    let doOnNext = (v, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);

      let haveValues =
        v0
        |> MutableOption.isNotEmpty
        && v1
        |> MutableOption.isNotEmpty
        && v2
        |> MutableOption.isNotEmpty
        && v3
        |> MutableOption.isNotEmpty
        && v4
        |> MutableOption.isNotEmpty
        && v5
        |> MutableOption.isNotEmpty
        && v6
        |> MutableOption.isNotEmpty;
      if (haveValues) {
        onNext(
          selector(
            v0 |> MutableOption.firstOrRaise,
            v1 |> MutableOption.firstOrRaise,
            v2 |> MutableOption.firstOrRaise,
            v3 |> MutableOption.firstOrRaise,
            v4 |> MutableOption.firstOrRaise,
            v5 |> MutableOption.firstOrRaise,
            v6 |> MutableOption.firstOrRaise,
          ),
        );
      };
      Lock.release(lock);
    };

    let doOnComplete = (d0, d1, d2, d3, d4, d5, exn) => {
      Lock.acquire(lock);
      switch (exn) {
      | Some(_) =>
        onComplete(exn);
        subscription |> AssignableDisposable.dispose;
      | None =>
        let shouldComplete =
          d0
          |> AssignableDisposable.isDisposed
          && d1
          |> AssignableDisposable.isDisposed
          && d2
          |> AssignableDisposable.isDisposed
          && d3
          |> AssignableDisposable.isDisposed
          && d4
          |> AssignableDisposable.isDisposed
          && d5
          |> AssignableDisposable.isDisposed;

        if (shouldComplete) {
          onComplete(None);
          subscription |> AssignableDisposable.dispose;
        };
      };
      Lock.release(lock);
    };

    s0
    |> AssignableDisposable.set(
         observable0
         |> subscribe(
              ~onNext=doOnNext(v0),
              ~onComplete=doOnComplete(s1, s2, s3, s4, s5, s6),
            ),
       );
    s1
    |> AssignableDisposable.set(
         observable1
         |> subscribe(
              ~onNext=doOnNext(v1),
              ~onComplete=doOnComplete(s0, s2, s3, s4, s5, s6),
            ),
       );
    s2
    |> AssignableDisposable.set(
         observable2
         |> subscribe(
              ~onNext=doOnNext(v2),
              ~onComplete=doOnComplete(s0, s1, s3, s4, s5, s6),
            ),
       );
    s3
    |> AssignableDisposable.set(
         observable3
         |> subscribe(
              ~onNext=doOnNext(v3),
              ~onComplete=doOnComplete(s0, s1, s2, s4, s5, s6),
            ),
       );
    s4
    |> AssignableDisposable.set(
         observable4
         |> subscribe(
              ~onNext=doOnNext(v4),
              ~onComplete=doOnComplete(s0, s1, s2, s3, s5, s6),
            ),
       );
    s5
    |> AssignableDisposable.set(
         observable5
         |> subscribe(
              ~onNext=doOnNext(v5),
              ~onComplete=doOnComplete(s0, s1, s2, s3, s4, s6),
            ),
       );
    s6
    |> AssignableDisposable.set(
         observable6
         |> subscribe(
              ~onNext=doOnNext(v6),
              ~onComplete=doOnComplete(s0, s1, s2, s3, s4, s5),
            ),
       );

    subscription
    |> AssignableDisposable.set(
         Disposable.create(() => {
           s0 |> AssignableDisposable.dispose;
           s1 |> AssignableDisposable.dispose;
           s2 |> AssignableDisposable.dispose;
           s3 |> AssignableDisposable.dispose;
           s4 |> AssignableDisposable.dispose;
           s5 |> AssignableDisposable.dispose;
           s6 |> AssignableDisposable.dispose;
           MutableOption.unset(v0);
           MutableOption.unset(v1);
           MutableOption.unset(v2);
           MutableOption.unset(v3);
           MutableOption.unset(v4);
           MutableOption.unset(v5);
           MutableOption.unset(v6);
         }),
       );
    subscription |> AssignableDisposable.toDisposable;
  });

let concat =
    /* FIXME: The default scheduler here should be a trampoline */
    (~scheduler=Scheduler.immediate, observables: list(t('a)))
    : t('a) =>
  create((~onNext, ~onComplete) => {
    let remaining = ref(observables);
    let subscription = AssignableDisposable.create();
    let rec scheduleSubscription = () => {
      let newSubscription =
        scheduler(() =>
          switch (remaining^) {
          | [hd, ...tail] =>
            Volatile.write(tail, remaining);
            doSubscribe(hd);
          | [] =>
            onComplete(None);
            subscription |> AssignableDisposable.dispose;
            Disposable.disposed;
          }
        );
      subscription |> AssignableDisposable.set(newSubscription);
    }
    and doSubscribe = observable =>
      observable
      |> subscribe(~onNext, ~onComplete=exn =>
           switch (exn) {
           | Some(_) =>
             onComplete(exn);
             subscription |> AssignableDisposable.dispose;
           | None => scheduleSubscription()
           }
         );
    scheduleSubscription();
    subscription |> AssignableDisposable.toDisposable;
  });

let defer = (f: unit => t('a)) : t('a) =>
  create((~onNext, ~onComplete) => f() |> subscribe(~onNext, ~onComplete));

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
  createWithObserver(observer => {
    let lifted = operator(observer);
    let subscription = observable |> subscribeObserver(lifted);
    Disposable.compose([subscription, lifted |> Observer.toDisposable]);
  });

let merge =
    /* FIXME: Should use a trampoline scheduler as the default for this function */
    (~scheduler as _=Scheduler.immediate, _: list(t('a)))
    : t('a) =>
  failwith("Not implemented");

let never = () : t('a) =>
  create((~onNext as _, ~onComplete as _) => Disposable.empty());

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
        | [hd, ...tail] =>
          onNext(hd);
          scheduler(loop(tail));
        | [] =>
          onComplete(None);
          Disposable.disposed;
        };
      scheduler(loop(list));
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

let retry = (shouldRetry, observable: t('a)) : t('a) =>
  (~onNext, ~onComplete) => {
    let subscription = AssignableDisposable.create();

    let rec setupSubscription = () => {
      let alreadyDisposed = subscription |> AssignableDisposable.isDisposed;

      if (! alreadyDisposed) {
        let newSubscription =
          observable
          |> subscribe(
               ~onNext,
               ~onComplete=exn => {
                 let shouldComplete =
                   switch (exn) {
                   | None => true
                   | Some(exn) => ! shouldRetry(exn)
                   };

                 shouldComplete ? onComplete(exn) : setupSubscription();
               },
             );
        subscription |> AssignableDisposable.set(newSubscription);
      };
    };
    setupSubscription();
    subscription |> AssignableDisposable.toDisposable;
  };

let startWithList =
    (~scheduler=Scheduler.immediate, values: list('a), observable: t('a)) =>
  concat([ofList(~scheduler, values), observable]);

let startWithValue =
    (~scheduler=Scheduler.immediate, value: 'a, observable: t('a)) =>
  concat([ofValue(~scheduler, value), observable]);
let combineLatest2 =
    (
      ~selector: ('a, 'b) => 'c,
      observable0: ObservableSource.t('a),
      observable1: ObservableSource.t('b),
    )
    : ObservableSource.t('c) =>
  ObservableSource.create((~onNext, ~onComplete) => {
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
      |> ObservableSource.subscribeWith(
           ~onNext=doOnNext(v),
           ~onComplete=doOnComplete(s),
         );

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
      observable0: ObservableSource.t('a),
      observable1: ObservableSource.t('b),
      observable2: ObservableSource.t('c),
    )
    : ObservableSource.t('d) =>
  ObservableSource.create((~onNext, ~onComplete) => {
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
      |> ObservableSource.subscribeWith(
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
      observable0: ObservableSource.t('a),
      observable1: ObservableSource.t('b),
      observable2: ObservableSource.t('c),
      observable3: ObservableSource.t('d),
    )
    : ObservableSource.t('e) =>
  ObservableSource.create((~onNext, ~onComplete) => {
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
      |> ObservableSource.subscribeWith(
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
      observable0: ObservableSource.t('a),
      observable1: ObservableSource.t('b),
      observable2: ObservableSource.t('c),
      observable3: ObservableSource.t('d),
      observable4: ObservableSource.t('e),
    )
    : ObservableSource.t('f) =>
  ObservableSource.create((~onNext, ~onComplete) => {
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
      |> ObservableSource.subscribeWith(
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
      observable0: ObservableSource.t('a),
      observable1: ObservableSource.t('b),
      observable2: ObservableSource.t('c),
      observable3: ObservableSource.t('d),
      observable4: ObservableSource.t('e),
      observable5: ObservableSource.t('f),
    )
    : ObservableSource.t('g) =>
  ObservableSource.create((~onNext, ~onComplete) => {
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
      |> ObservableSource.subscribeWith(
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
      observable0: ObservableSource.t('a),
      observable1: ObservableSource.t('b),
      observable2: ObservableSource.t('c),
      observable3: ObservableSource.t('d),
      observable4: ObservableSource.t('e),
      observable5: ObservableSource.t('f),
      observable6: ObservableSource.t('g),
    )
    : ObservableSource.t('h) =>
  ObservableSource.create((~onNext, ~onComplete) => {
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
      |> ObservableSource.subscribeWith(
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
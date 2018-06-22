let combineLatest2 = {
  let doOnNext = {
    let tryOnNext = ((selector, v0, v1, _), subscriber) => {
      let haveValues =
        MutableOption.isNotEmpty(v0) && MutableOption.isNotEmpty(v1);
      if (haveValues) {
        let next =
          try (selector(MutableOption.get(v0), MutableOption.get(v1))) {
          | exn =>
            subscriber |> Subscriber.complete(~exn);
            Functions.returnUnit();
          };
        subscriber |> Subscriber.next(next);
      };
    };

    ((_, _, _, lock) as ctx, v, _, subscriber, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      Functions.earlyReturnsUnit2(tryOnNext, ctx, subscriber);
      Lock.release(lock);
    };
  };

  let doOnComplete = ((_, _, _, lock), _, other, subscriber, exn) => {
    Lock.acquire(lock);
    switch (exn) {
    | Some(_) => subscriber |> Subscriber.complete(~exn?)
    | None =>
      let shouldComplete = CompositeDisposable.isDisposed(other^);
      if (shouldComplete) {
        subscriber |> Subscriber.complete(~exn?);
      };
    };
    Lock.release(lock);
  };

  let doSubscribe = (ctx, v, s, subscriber, observable) =>
    ObservableSource.subscribeWith4(
      ~onNext=doOnNext,
      ~onComplete=doOnComplete,
      ctx,
      v,
      s,
      subscriber,
      observable,
    );

  (~selector, observable0, observable1) =>
    ObservableSource.create(subscriber => {
      let (v0, v1) = (MutableOption.create(), MutableOption.create());
      let lock = Lock.create();

      let ctx = (selector, v0, v1, lock);

      let (s0, s1) = CompositeDisposable.(ref(disposed), ref(disposed));
      s0 := observable0 |> doSubscribe(ctx, v0, s1, subscriber);
      s1 := observable1 |> doSubscribe(ctx, v1, s0, subscriber);
      let (s0, s1) = (s0^, s1^);

      subscriber
      |> Subscriber.addCompositeDisposable(s0)
      |> Subscriber.addCompositeDisposable(s1)
      |> Subscriber.addTeardown1(MutableOption.unset, v0)
      |> Subscriber.addTeardown1(MutableOption.unset, v1)
      |> ignore;
    });
};

let combineLatest3 = {
  let doOnNext = {
    let tryOnNext = ((selector, v0, v1, v2, _), subscriber) => {
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
            subscriber |> Subscriber.complete(~exn);
            Functions.returnUnit();
          };
        subscriber |> Subscriber.next(next);
      };
    };

    ((_, _, _, _, lock) as ctx, v, _, _, subscriber, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      Functions.earlyReturnsUnit2(tryOnNext, ctx, subscriber);
      Lock.release(lock);
    };
  };

  let doOnComplete = ((_, _, _, _, lock), _, d0, d1, subscriber, exn) => {
    Lock.acquire(lock);
    switch (exn) {
    | Some(_) => subscriber |> Subscriber.complete(~exn?)
    | None =>
      let shouldComplete =
        CompositeDisposable.isDisposed(d0^)
        && CompositeDisposable.isDisposed(d1^);

      if (shouldComplete) {
        subscriber |> Subscriber.complete(~exn?);
      };
    };
    Lock.release(lock);
  };

  let doSubscribe = (ctx, v, s0, s1, subscriber, observable) =>
    ObservableSource.subscribeWith5(
      ~onNext=doOnNext,
      ~onComplete=doOnComplete,
      ctx,
      v,
      s0,
      s1,
      subscriber,
      observable,
    );

  (~selector, observable0, observable1, observable2) =>
    ObservableSource.create(subscriber => {
      let (v0, v1, v2) = (
        MutableOption.create(),
        MutableOption.create(),
        MutableOption.create(),
      );
      let lock = Lock.create();

      let ctx = (selector, v0, v1, v2, lock);

      let (s0, s1, s2) =
        CompositeDisposable.(ref(disposed), ref(disposed), ref(disposed));
      s0 := observable0 |> doSubscribe(ctx, v0, s1, s2, subscriber);
      s1 := observable1 |> doSubscribe(ctx, v1, s0, s2, subscriber);
      s2 := observable2 |> doSubscribe(ctx, v2, s0, s1, subscriber);
      let (s0, s1, s2) = (s0^, s1^, s2^);

      subscriber
      |> Subscriber.addCompositeDisposable(s0)
      |> Subscriber.addCompositeDisposable(s1)
      |> Subscriber.addCompositeDisposable(s2)
      |> Subscriber.addTeardown1(MutableOption.unset, v0)
      |> Subscriber.addTeardown1(MutableOption.unset, v1)
      |> Subscriber.addTeardown1(MutableOption.unset, v2)
      |> ignore;
    });
};

let combineLatest4 =
    (
      ~selector: ('a, 'b, 'c, 'd) => 'e,
      observable0: ObservableSource.t('a),
      observable1: ObservableSource.t('b),
      observable2: ObservableSource.t('c),
      observable3: ObservableSource.t('d),
    )
    : ObservableSource.t('e) =>
  ObservableSource.create(subscriber => {
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
              subscriber |> Subscriber.complete(~exn);
              Functions.returnUnit();
            };
          subscriber |> Subscriber.next(next);
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
      | Some(_) => subscriber |> Subscriber.complete(~exn?)
      | None =>
        let shouldComplete =
          CompositeDisposable.isDisposed(d0^)
          && CompositeDisposable.isDisposed(d1^)
          && CompositeDisposable.isDisposed(d2^);

        if (shouldComplete) {
          subscriber |> Subscriber.complete(~exn?);
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

    let (s0, s1, s2, s3) =
      CompositeDisposable.(
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
      );

    s0 := doSubscribe(observable0, v0, s1, s2, s3);
    s1 := doSubscribe(observable1, v1, s0, s2, s3);
    s2 := doSubscribe(observable2, v2, s0, s1, s3);
    s3 := doSubscribe(observable3, v3, s0, s1, s2);

    let (s0, s1, s2, s3) = (s0^, s1^, s2^, s3^);

    subscriber
    |> Subscriber.addCompositeDisposable(s0)
    |> Subscriber.addCompositeDisposable(s1)
    |> Subscriber.addCompositeDisposable(s2)
    |> Subscriber.addCompositeDisposable(s3)
    |> Subscriber.addTeardown1(MutableOption.unset, v0)
    |> Subscriber.addTeardown1(MutableOption.unset, v1)
    |> Subscriber.addTeardown1(MutableOption.unset, v2)
    |> Subscriber.addTeardown1(MutableOption.unset, v3)
    |> ignore;
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
  ObservableSource.create(subscriber => {
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
              subscriber |> Subscriber.complete(~exn);
              Functions.returnUnit();
            };
          subscriber |> Subscriber.next(next);
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
      | Some(_) => subscriber |> Subscriber.complete(~exn?)
      | None =>
        let shouldComplete =
          CompositeDisposable.isDisposed(d0^)
          && CompositeDisposable.isDisposed(d1^)
          && CompositeDisposable.isDisposed(d2^)
          && CompositeDisposable.isDisposed(d3^);

        if (shouldComplete) {
          subscriber |> Subscriber.complete(~exn?);
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

    let (s0, s1, s2, s3, s4) =
      CompositeDisposable.(
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
      );

    s0 := doSubscribe(observable0, v0, s1, s2, s3, s4);
    s1 := doSubscribe(observable1, v1, s0, s2, s3, s4);
    s2 := doSubscribe(observable2, v2, s0, s1, s3, s4);
    s3 := doSubscribe(observable3, v3, s0, s1, s2, s4);
    s4 := doSubscribe(observable4, v4, s0, s1, s2, s3);

    let (s0, s1, s2, s3, s4) = (s0^, s1^, s2^, s3^, s4^);

    subscriber
    |> Subscriber.addCompositeDisposable(s0)
    |> Subscriber.addCompositeDisposable(s1)
    |> Subscriber.addCompositeDisposable(s2)
    |> Subscriber.addCompositeDisposable(s3)
    |> Subscriber.addCompositeDisposable(s4)
    |> Subscriber.addTeardown1(MutableOption.unset, v0)
    |> Subscriber.addTeardown1(MutableOption.unset, v1)
    |> Subscriber.addTeardown1(MutableOption.unset, v2)
    |> Subscriber.addTeardown1(MutableOption.unset, v3)
    |> Subscriber.addTeardown1(MutableOption.unset, v4)
    |> ignore;
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
  ObservableSource.create(subscriber => {
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
              subscriber |> Subscriber.complete(~exn);
              Functions.returnUnit();
            };
          subscriber |> Subscriber.next(next);
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
      | Some(_) => subscriber |> Subscriber.complete(~exn?)
      | None =>
        let shouldComplete =
          CompositeDisposable.isDisposed(d0^)
          && CompositeDisposable.isDisposed(d1^)
          && CompositeDisposable.isDisposed(d2^)
          && CompositeDisposable.isDisposed(d3^)
          && CompositeDisposable.isDisposed(d4^);

        if (shouldComplete) {
          subscriber |> Subscriber.complete(~exn?);
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

    let (s0, s1, s2, s3, s4, s5) =
      CompositeDisposable.(
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
      );

    s0 := doSubscribe(observable0, v0, s1, s2, s3, s4, s5);
    s1 := doSubscribe(observable1, v1, s0, s2, s3, s4, s5);
    s2 := doSubscribe(observable2, v2, s0, s1, s3, s4, s5);
    s3 := doSubscribe(observable3, v3, s0, s1, s2, s4, s5);
    s4 := doSubscribe(observable4, v4, s0, s1, s2, s3, s5);
    s5 := doSubscribe(observable5, v5, s0, s1, s2, s3, s4);

    let (s0, s1, s2, s3, s4, s5) = (s0^, s1^, s2^, s3^, s4^, s5^);

    subscriber
    |> Subscriber.addCompositeDisposable(s0)
    |> Subscriber.addCompositeDisposable(s1)
    |> Subscriber.addCompositeDisposable(s2)
    |> Subscriber.addCompositeDisposable(s3)
    |> Subscriber.addCompositeDisposable(s4)
    |> Subscriber.addCompositeDisposable(s5)
    |> Subscriber.addTeardown1(MutableOption.unset, v0)
    |> Subscriber.addTeardown1(MutableOption.unset, v1)
    |> Subscriber.addTeardown1(MutableOption.unset, v2)
    |> Subscriber.addTeardown1(MutableOption.unset, v3)
    |> Subscriber.addTeardown1(MutableOption.unset, v4)
    |> Subscriber.addTeardown1(MutableOption.unset, v5)
    |> ignore;
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
  ObservableSource.create(subscriber => {
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
              subscriber |> Subscriber.complete(~exn);
              Functions.returnUnit();
            };
          subscriber |> Subscriber.next(next);
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
      | Some(_) => subscriber |> Subscriber.complete(~exn?)
      | None =>
        let shouldComplete =
          CompositeDisposable.isDisposed(d0^)
          && CompositeDisposable.isDisposed(d1^)
          && CompositeDisposable.isDisposed(d2^)
          && CompositeDisposable.isDisposed(d3^)
          && CompositeDisposable.isDisposed(d4^)
          && CompositeDisposable.isDisposed(d5^);

        if (shouldComplete) {
          subscriber |> Subscriber.complete(~exn?);
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

    let (s0, s1, s2, s3, s4, s5, s6) =
      CompositeDisposable.(
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
      );

    s0 := doSubscribe(observable0, v0, s1, s2, s3, s4, s5, s6);
    s1 := doSubscribe(observable1, v1, s0, s2, s3, s4, s5, s6);
    s2 := doSubscribe(observable2, v2, s0, s1, s3, s4, s5, s6);
    s3 := doSubscribe(observable3, v3, s0, s1, s2, s4, s5, s6);
    s4 := doSubscribe(observable4, v4, s0, s1, s2, s3, s5, s6);
    s5 := doSubscribe(observable5, v5, s0, s1, s2, s3, s4, s6);
    s6 := doSubscribe(observable6, v6, s0, s1, s2, s3, s4, s5);

    let (s0, s1, s2, s3, s4, s5, s6) = (s0^, s1^, s2^, s3^, s4^, s5^, s6^);

    subscriber
    |> Subscriber.addCompositeDisposable(s0)
    |> Subscriber.addCompositeDisposable(s1)
    |> Subscriber.addCompositeDisposable(s2)
    |> Subscriber.addCompositeDisposable(s3)
    |> Subscriber.addCompositeDisposable(s4)
    |> Subscriber.addCompositeDisposable(s5)
    |> Subscriber.addCompositeDisposable(s6)
    |> Subscriber.addTeardown1(MutableOption.unset, v0)
    |> Subscriber.addTeardown1(MutableOption.unset, v1)
    |> Subscriber.addTeardown1(MutableOption.unset, v2)
    |> Subscriber.addTeardown1(MutableOption.unset, v3)
    |> Subscriber.addTeardown1(MutableOption.unset, v4)
    |> Subscriber.addTeardown1(MutableOption.unset, v5)
    |> Subscriber.addTeardown1(MutableOption.unset, v6)
    |> ignore;
  });
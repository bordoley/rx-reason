let combineLatest2 = {
  let doOnNext = {
    let tryOnNext = (selector, v0, v1, subscriber) => {
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

    (selector, v0, v1, lock, v, _, subscriber, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      Functions.earlyReturnsUnit4(tryOnNext, selector, v0, v1, subscriber);
      Lock.release(lock);
    };
  };

  let doOnComplete = (_, _, _, lock, _, other, subscriber, exn) => {
    Lock.acquire(lock);
    switch (exn) {
    | Some(_) => subscriber |> Subscriber.complete(~exn?)
    | None =>
      let shouldComplete = Disposable.isDisposed(other^);
      if (shouldComplete) {
        subscriber |> Subscriber.complete(~exn?);
      };
    };
    Lock.release(lock);
  };

  let doSubscribe = (selector, v0, v1, lock, v, s, subscriber, observable) =>
    ObservableSource.subscribeWith7(
      ~onNext=doOnNext,
      ~onComplete=doOnComplete,
      selector,
      v0,
      v1,
      lock,
      v,
      s,
      subscriber,
      observable,
    );

  let source = ((selector, observable0, observable1), subscriber) => {
    let (v0, v1) = (MutableOption.create(), MutableOption.create());
    let lock = Lock.create();

    let (s0, s1) = Disposable.(ref(disposed), ref(disposed));
    s0 :=
      observable0 |> doSubscribe(selector, v0, v1, lock, v0, s1, subscriber);
    s1 :=
      observable1 |> doSubscribe(selector, v0, v1, lock, v1, s0, subscriber);
    let (s0, s1) = (s0^, s1^);

    subscriber
    |> Subscriber.addTeardown1(Disposable.dispose, s0)
    |> Subscriber.addTeardown1(Disposable.dispose, s1)
    |> Subscriber.addTeardown1(MutableOption.unset, v0)
    |> Subscriber.addTeardown1(MutableOption.unset, v1)
    |> ignore;
  };

  (~selector, observable0, observable1) =>
    ObservableSource.create1(source, (selector, observable0, observable1));
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
        Disposable.isDisposed(d0^) && Disposable.isDisposed(d1^);

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

  let source =
      ((selector, observable0, observable1, observable2), subscriber) => {
    let (v0, v1, v2) = (
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
    );
    let lock = Lock.create();

    let ctx = (selector, v0, v1, v2, lock);

    let (s0, s1, s2) =
      Disposable.(ref(disposed), ref(disposed), ref(disposed));
    s0 := observable0 |> doSubscribe(ctx, v0, s1, s2, subscriber);
    s1 := observable1 |> doSubscribe(ctx, v1, s0, s2, subscriber);
    s2 := observable2 |> doSubscribe(ctx, v2, s0, s1, subscriber);
    let (s0, s1, s2) = (s0^, s1^, s2^);

    subscriber
    |> Subscriber.addTeardown1(Disposable.dispose, s0)
    |> Subscriber.addTeardown1(Disposable.dispose, s1)
    |> Subscriber.addTeardown1(Disposable.dispose, s2)
    |> Subscriber.addTeardown1(MutableOption.unset, v0)
    |> Subscriber.addTeardown1(MutableOption.unset, v1)
    |> Subscriber.addTeardown1(MutableOption.unset, v2)
    |> ignore;
  };

  (~selector, observable0, observable1, observable2) =>
    ObservableSource.create1(
      source,
      (selector, observable0, observable1, observable2),
    );
};

let combineLatest4 = {
  let doOnNext = {
    let tryOnNext = ((selector, v0, v1, v2, v3, _), subscriber) => {
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
    };

    ((_, _, _, _, _, lock) as ctx, v, _, _, _, subscriber, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      Functions.earlyReturnsUnit2(tryOnNext, ctx, subscriber);
      Lock.release(lock);
    };
  };

  let doOnComplete = ((_, _, _, _, _, lock), _, d0, d1, d2, subscriber, exn) => {
    Lock.acquire(lock);
    switch (exn) {
    | Some(_) => subscriber |> Subscriber.complete(~exn?)
    | None =>
      let shouldComplete =
        Disposable.isDisposed(d0^)
        && Disposable.isDisposed(d1^)
        && Disposable.isDisposed(d2^);

      if (shouldComplete) {
        subscriber |> Subscriber.complete(~exn?);
      };
    };
    Lock.release(lock);
  };

  let doSubscribe = (ctx, v, s0, s1, s2, subscriber, observable) =>
    ObservableSource.subscribeWith6(
      ~onNext=doOnNext,
      ~onComplete=doOnComplete,
      ctx,
      v,
      s0,
      s1,
      s2,
      subscriber,
      observable,
    );

  let source =
      (
        (selector, observable0, observable1, observable2, observable3),
        subscriber,
      ) => {
    let (v0, v1, v2, v3) = (
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
      MutableOption.create(),
    );
    let lock = Lock.create();

    let ctx = (selector, v0, v1, v2, v3, lock);

    let (s0, s1, s2, s3) =
      Disposable.(
        ref(disposed),
        ref(disposed),
        ref(disposed),
        ref(disposed),
      );
    s0 := observable0 |> doSubscribe(ctx, v0, s1, s2, s3, subscriber);
    s1 := observable1 |> doSubscribe(ctx, v1, s0, s2, s3, subscriber);
    s2 := observable2 |> doSubscribe(ctx, v2, s0, s1, s3, subscriber);
    s3 := observable3 |> doSubscribe(ctx, v3, s0, s1, s2, subscriber);
    let (s0, s1, s2, s3) = (s0^, s1^, s2^, s3^);

    subscriber
    |> Subscriber.addTeardown1(Disposable.dispose, s0)
    |> Subscriber.addTeardown1(Disposable.dispose, s1)
    |> Subscriber.addTeardown1(Disposable.dispose, s2)
    |> Subscriber.addTeardown1(Disposable.dispose, s3)
    |> Subscriber.addTeardown1(MutableOption.unset, v0)
    |> Subscriber.addTeardown1(MutableOption.unset, v1)
    |> Subscriber.addTeardown1(MutableOption.unset, v2)
    |> Subscriber.addTeardown1(MutableOption.unset, v3)
    |> ignore;
  };

  (~selector, observable0, observable1, observable2, observable3) =>
    ObservableSource.create1(
      source,
      (selector, observable0, observable1, observable2, observable3),
    );
};

let combineLatest5 = {
  let doOnNext = {
    let tryOnNext = ((selector, v0, v1, v2, v3, v4, _), subscriber) => {
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
    };

    ((_, _, _, _, _, _, lock) as ctx, v, _, _, _, _, subscriber, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      Functions.earlyReturnsUnit2(tryOnNext, ctx, subscriber);
      Lock.release(lock);
    };
  };

  let doOnComplete =
      ((_, _, _, _, _, _, lock), _, d0, d1, d2, d3, subscriber, exn) => {
    Lock.acquire(lock);
    switch (exn) {
    | Some(_) => subscriber |> Subscriber.complete(~exn?)
    | None =>
      let shouldComplete =
        Disposable.isDisposed(d0^)
        && Disposable.isDisposed(d1^)
        && Disposable.isDisposed(d2^)
        && Disposable.isDisposed(d3^);

      if (shouldComplete) {
        subscriber |> Subscriber.complete(~exn?);
      };
    };
    Lock.release(lock);
  };

  let doSubscribe = (ctx, v, s0, s1, s2, s3, subscriber, observable) =>
    ObservableSource.subscribeWith7(
      ~onNext=doOnNext,
      ~onComplete=doOnComplete,
      ctx,
      v,
      s0,
      s1,
      s2,
      s3,
      subscriber,
      observable,
    );

  (~selector, observable0, observable1, observable2, observable3, observable4) =>
    ObservableSource.create((_, subscriber) => {
      let (v0, v1, v2, v3, v4) = (
        MutableOption.create(),
        MutableOption.create(),
        MutableOption.create(),
        MutableOption.create(),
        MutableOption.create(),
      );
      let lock = Lock.create();

      let ctx = (selector, v0, v1, v2, v3, v4, lock);

      let (s0, s1, s2, s3, s4) =
        Disposable.(
          ref(disposed),
          ref(disposed),
          ref(disposed),
          ref(disposed),
          ref(disposed),
        );
      s0 := observable0 |> doSubscribe(ctx, v0, s1, s2, s3, s4, subscriber);
      s1 := observable1 |> doSubscribe(ctx, v1, s0, s2, s3, s4, subscriber);
      s2 := observable2 |> doSubscribe(ctx, v2, s0, s1, s3, s4, subscriber);
      s3 := observable3 |> doSubscribe(ctx, v3, s0, s1, s2, s4, subscriber);
      s4 := observable4 |> doSubscribe(ctx, v4, s0, s1, s2, s3, subscriber);
      let (s0, s1, s2, s3, s4) = (s0^, s1^, s2^, s3^, s4^);

      subscriber
      |> Subscriber.addTeardown1(Disposable.dispose, s0)
      |> Subscriber.addTeardown1(Disposable.dispose, s1)
      |> Subscriber.addTeardown1(Disposable.dispose, s2)
      |> Subscriber.addTeardown1(Disposable.dispose, s3)
      |> Subscriber.addTeardown1(Disposable.dispose, s4)
      |> Subscriber.addTeardown1(MutableOption.unset, v0)
      |> Subscriber.addTeardown1(MutableOption.unset, v1)
      |> Subscriber.addTeardown1(MutableOption.unset, v2)
      |> Subscriber.addTeardown1(MutableOption.unset, v3)
      |> Subscriber.addTeardown1(MutableOption.unset, v4)
      |> ignore;
    });
};

let combineLatest6 = {
  let doOnNext = {
    let tryOnNext = ((selector, v0, v1, v2, v3, v4, v5, _), subscriber) => {
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
    };

    ((_, _, _, _, _, _, _, lock) as ctx, v, _, subscriber, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      Functions.earlyReturnsUnit2(tryOnNext, ctx, subscriber);
      Lock.release(lock);
    };
  };

  let doOnComplete =
      (
        (_, _, _, _, _, _, _, lock),
        _,
        (d0, d1, d2, d3, d4),
        subscriber,
        exn,
      ) => {
    Lock.acquire(lock);
    switch (exn) {
    | Some(_) => subscriber |> Subscriber.complete(~exn?)
    | None =>
      let shouldComplete =
        Disposable.isDisposed(d0^)
        && Disposable.isDisposed(d1^)
        && Disposable.isDisposed(d2^)
        && Disposable.isDisposed(d3^)
        && Disposable.isDisposed(d4^);
      if (shouldComplete) {
        subscriber |> Subscriber.complete(~exn?);
      };
    };
    Lock.release(lock);
  };

  let doSubscribe = (ctx, v, s0, s1, s2, s3, s4, subscriber, observable) =>
    ObservableSource.subscribeWith4(
      ~onNext=doOnNext,
      ~onComplete=doOnComplete,
      ctx,
      v,
      (s0, s1, s2, s3, s4),
      subscriber,
      observable,
    );

  (
    ~selector,
    observable0,
    observable1,
    observable2,
    observable3,
    observable4,
    observable5,
  ) =>
    ObservableSource.create((_, subscriber) => {
      let (v0, v1, v2, v3, v4, v5) = (
        MutableOption.create(),
        MutableOption.create(),
        MutableOption.create(),
        MutableOption.create(),
        MutableOption.create(),
        MutableOption.create(),
      );
      let lock = Lock.create();

      let ctx = (selector, v0, v1, v2, v3, v4, v5, lock);

      let (s0, s1, s2, s3, s4, s5) =
        Disposable.(
          ref(disposed),
          ref(disposed),
          ref(disposed),
          ref(disposed),
          ref(disposed),
          ref(disposed),
        );
      s0 :=
        observable0 |> doSubscribe(ctx, v0, s1, s2, s3, s4, s5, subscriber);
      s1 :=
        observable1 |> doSubscribe(ctx, v1, s0, s2, s3, s4, s5, subscriber);
      s2 :=
        observable2 |> doSubscribe(ctx, v2, s0, s1, s3, s4, s5, subscriber);
      s3 :=
        observable3 |> doSubscribe(ctx, v3, s0, s1, s2, s4, s5, subscriber);
      s4 :=
        observable4 |> doSubscribe(ctx, v4, s0, s1, s2, s3, s5, subscriber);
      s5 :=
        observable5 |> doSubscribe(ctx, v5, s0, s1, s2, s3, s4, subscriber);
      let (s0, s1, s2, s3, s4, s5) = (s0^, s1^, s2^, s3^, s4^, s5^);

      subscriber
      |> Subscriber.addTeardown1(Disposable.dispose, s0)
      |> Subscriber.addTeardown1(Disposable.dispose, s1)
      |> Subscriber.addTeardown1(Disposable.dispose, s2)
      |> Subscriber.addTeardown1(Disposable.dispose, s3)
      |> Subscriber.addTeardown1(Disposable.dispose, s4)
      |> Subscriber.addTeardown1(Disposable.dispose, s5)
      |> Subscriber.addTeardown1(MutableOption.unset, v0)
      |> Subscriber.addTeardown1(MutableOption.unset, v1)
      |> Subscriber.addTeardown1(MutableOption.unset, v2)
      |> Subscriber.addTeardown1(MutableOption.unset, v3)
      |> Subscriber.addTeardown1(MutableOption.unset, v4)
      |> Subscriber.addTeardown1(MutableOption.unset, v5)
      |> ignore;
    });
};

let combineLatest7 = {
  let doOnNext = {
    let tryOnNext = ((selector, v0, v1, v2, v3, v4, v5, v6, _), subscriber) => {
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
    };

    ((_, _, _, _, _, _, _, _, lock) as ctx, v, _, subscriber, next) => {
      Lock.acquire(lock);
      MutableOption.set(next, v);
      Functions.earlyReturnsUnit2(tryOnNext, ctx, subscriber);
      Lock.release(lock);
    };
  };

  let doOnComplete =
      (
        (_, _, _, _, _, _, _, _, lock),
        _,
        (d0, d1, d2, d3, d4, d5),
        subscriber,
        exn,
      ) => {
    Lock.acquire(lock);
    switch (exn) {
    | Some(_) => subscriber |> Subscriber.complete(~exn?)
    | None =>
      let shouldComplete =
        Disposable.isDisposed(d0^)
        && Disposable.isDisposed(d1^)
        && Disposable.isDisposed(d2^)
        && Disposable.isDisposed(d3^)
        && Disposable.isDisposed(d4^)
        && Disposable.isDisposed(d5^);
      if (shouldComplete) {
        subscriber |> Subscriber.complete(~exn?);
      };
    };
    Lock.release(lock);
  };

  let doSubscribe = (ctx, v, s0, s1, s2, s3, s4, s5, subscriber, observable) =>
    ObservableSource.subscribeWith4(
      ~onNext=doOnNext,
      ~onComplete=doOnComplete,
      ctx,
      v,
      (s0, s1, s2, s3, s4, s5),
      subscriber,
      observable,
    );

  (
    ~selector,
    observable0,
    observable1,
    observable2,
    observable3,
    observable4,
    observable5,
    observable6,
  ) =>
    ObservableSource.create((_, subscriber) => {
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

      let ctx = (selector, v0, v1, v2, v3, v4, v5, v6, lock);

      let (s0, s1, s2, s3, s4, s5, s6) =
        Disposable.(
          ref(disposed),
          ref(disposed),
          ref(disposed),
          ref(disposed),
          ref(disposed),
          ref(disposed),
          ref(disposed),
        );
      s0 :=
        observable0
        |> doSubscribe(ctx, v0, s1, s2, s3, s4, s5, s6, subscriber);
      s1 :=
        observable1
        |> doSubscribe(ctx, v1, s0, s2, s3, s4, s5, s6, subscriber);
      s2 :=
        observable2
        |> doSubscribe(ctx, v2, s0, s1, s3, s4, s5, s6, subscriber);
      s3 :=
        observable3
        |> doSubscribe(ctx, v3, s0, s1, s2, s4, s5, s6, subscriber);
      s4 :=
        observable4
        |> doSubscribe(ctx, v4, s0, s1, s2, s3, s5, s6, subscriber);
      s5 :=
        observable5
        |> doSubscribe(ctx, v5, s0, s1, s2, s3, s4, s6, subscriber);
      s6 :=
        observable6
        |> doSubscribe(ctx, v6, s0, s1, s2, s3, s4, s5, subscriber);

      let (s0, s1, s2, s3, s4, s5, s6) = (s0^, s1^, s2^, s3^, s4^, s5^, s6^);

      subscriber
      |> Subscriber.addTeardown1(Disposable.dispose, s0)
      |> Subscriber.addTeardown1(Disposable.dispose, s1)
      |> Subscriber.addTeardown1(Disposable.dispose, s2)
      |> Subscriber.addTeardown1(Disposable.dispose, s3)
      |> Subscriber.addTeardown1(Disposable.dispose, s4)
      |> Subscriber.addTeardown1(Disposable.dispose, s5)
      |> Subscriber.addTeardown1(Disposable.dispose, s6)
      |> Subscriber.addTeardown1(MutableOption.unset, v0)
      |> Subscriber.addTeardown1(MutableOption.unset, v1)
      |> Subscriber.addTeardown1(MutableOption.unset, v2)
      |> Subscriber.addTeardown1(MutableOption.unset, v3)
      |> Subscriber.addTeardown1(MutableOption.unset, v4)
      |> Subscriber.addTeardown1(MutableOption.unset, v5)
      |> Subscriber.addTeardown1(MutableOption.unset, v6)
      |> ignore;
    });
};
let publish = {
  let teardown = (subscription, active) => {
    subscription |> RxDisposable.dispose;
    RxAtomic.set(active, false);
  };

  (~onNext, ~onComplete, observable) => {
    let connection = RxAtomic.make(RxDisposable.disposed);
    let active = RxAtomic.make(false);

    () => {
      if (! RxAtomic.exchange(active, true)) {
        let subscriber = RxSubscriber.create(~onNext, ~onComplete);
        observable |> RxObservable.subscribeWith(subscriber);
        let newConnection =
          RxDisposable.create2(
            teardown,
            subscriber |> RxSubscriber.asDisposable,
            active,
          );

        RxAtomic.set(connection, newConnection);
      };
      RxAtomic.get(connection);
    };
  };
};

let publish1 = {
  let teardown = (subscription, active) => {
    subscription |> RxDisposable.dispose;
    RxAtomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, observable) => {
    let connection = RxAtomic.make(RxDisposable.disposed);
    let active = RxAtomic.make(false);

    () => {
      if (! RxAtomic.exchange(active, true)) {
        let subscriber = RxSubscriber.create1(~onNext, ~onComplete, ctx0);
        observable |> RxObservable.subscribeWith(subscriber);
        let newConnection =
          RxDisposable.create2(
            teardown,
            subscriber |> RxSubscriber.asDisposable,
            active,
          );

        RxAtomic.set(connection, newConnection);
      };
      RxAtomic.get(connection);
    };
  };
};

let publish2 = {
  let teardown = (subscription, active) => {
    subscription |> RxDisposable.dispose;
    RxAtomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, observable) => {
    let connection = RxAtomic.make(RxDisposable.disposed);
    let active = RxAtomic.make(false);

    () => {
      if (! RxAtomic.exchange(active, true)) {
        let subscriber =
          RxSubscriber.create2(~onNext, ~onComplete, ctx0, ctx1);
        observable |> RxObservable.subscribeWith(subscriber);
        let newConnection =
          RxDisposable.create2(
            teardown,
            subscriber |> RxSubscriber.asDisposable,
            active,
          );

        RxAtomic.set(connection, newConnection);
      };
      RxAtomic.get(connection);
    };
  };
};

let publish3 = {
  let teardown = (subscription, active) => {
    subscription |> RxDisposable.dispose;
    RxAtomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, observable) => {
    let connection = RxAtomic.make(RxDisposable.disposed);
    let active = RxAtomic.make(false);

    () => {
      if (! RxAtomic.exchange(active, true)) {
        let subscriber =
          RxSubscriber.create3(~onNext, ~onComplete, ctx0, ctx1, ctx2);
        observable |> RxObservable.subscribeWith(subscriber);
        let newConnection =
          RxDisposable.create2(
            teardown,
            subscriber |> RxSubscriber.asDisposable,
            active,
          );

        RxAtomic.set(connection, newConnection);
      };
      RxAtomic.get(connection);
    };
  };
};

let publish4 = {
  let teardown = (subscription, active) => {
    subscription |> RxDisposable.dispose;
    RxAtomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, observable) => {
    let connection = RxAtomic.make(RxDisposable.disposed);
    let active = RxAtomic.make(false);

    () => {
      if (! RxAtomic.exchange(active, true)) {
        let subscriber =
          RxSubscriber.create4(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3);
        observable |> RxObservable.subscribeWith(subscriber);
        let newConnection =
          RxDisposable.create2(
            teardown,
            subscriber |> RxSubscriber.asDisposable,
            active,
          );

        RxAtomic.set(connection, newConnection);
      };
      RxAtomic.get(connection);
    };
  };
};

let publish5 = {
  let teardown = (subscription, active) => {
    subscription |> RxDisposable.dispose;
    RxAtomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, observable) => {
    let connection = RxAtomic.make(RxDisposable.disposed);
    let active = RxAtomic.make(false);

    () => {
      if (! RxAtomic.exchange(active, true)) {
        let subscriber =
          RxSubscriber.create5(
            ~onNext,
            ~onComplete,
            ctx0,
            ctx1,
            ctx2,
            ctx3,
            ctx4,
          );
        observable |> RxObservable.subscribeWith(subscriber);
        let newConnection =
          RxDisposable.create2(
            teardown,
            subscriber |> RxSubscriber.asDisposable,
            active,
          );

        RxAtomic.set(connection, newConnection);
      };
      RxAtomic.get(connection);
    };
  };
};

let publish6 = {
  let teardown = (subscription, active) => {
    subscription |> RxDisposable.dispose;
    RxAtomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, observable) => {
    let connection = RxAtomic.make(RxDisposable.disposed);
    let active = RxAtomic.make(false);

    () => {
      if (! RxAtomic.exchange(active, true)) {
        let subscriber =
          RxSubscriber.create6(
            ~onNext,
            ~onComplete,
            ctx0,
            ctx1,
            ctx2,
            ctx3,
            ctx4,
            ctx5,
          );
        observable |> RxObservable.subscribeWith(subscriber);
        let newConnection =
          RxDisposable.create2(
            teardown,
            subscriber |> RxSubscriber.asDisposable,
            active,
          );

        RxAtomic.set(connection, newConnection);
      };
      RxAtomic.get(connection);
    };
  };
};

let publish7 = {
  let teardown = (subscription, active) => {
    subscription |> RxDisposable.dispose;
    RxAtomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, observable) => {
    let connection = RxAtomic.make(RxDisposable.disposed);
    let active = RxAtomic.make(false);

    () => {
      if (! RxAtomic.exchange(active, true)) {
        let subscriber =
          RxSubscriber.create7(
            ~onNext,
            ~onComplete,
            ctx0,
            ctx1,
            ctx2,
            ctx3,
            ctx4,
            ctx5,
            ctx6,
          );
        observable |> RxObservable.subscribeWith(subscriber);
        let newConnection =
          RxDisposable.create2(
            teardown,
            subscriber |> RxSubscriber.asDisposable,
            active,
          );

        RxAtomic.set(connection, newConnection);
      };
      RxAtomic.get(connection);
    };
  };
};
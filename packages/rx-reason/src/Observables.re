let concat = ConcatObservable.concat;
let defer = DeferObservable.defer;
let empty = EmptyObservable.empty;
let merge = MergeObservable.merge;
let ofAbsoluteTimeNotifications = OfObservable.ofAbsoluteTimeNotifications;
let ofList = OfObservable.ofList;
let ofNotifications = OfObservable.ofNotifications;
let ofRelativeTimeNotifications = OfObservable.ofRelativeTimeNotifications;
let ofValue = OfObservable.ofValue;

let publish = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscriber = Subscriber.create(~onNext, ~onComplete);
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(
            teardown,
            subscriber |> Subscriber.asDisposable,
            active,
          );

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publish1 = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscriber =
          Subscriber.create1(~onNext, ~onComplete, ctx0);
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(
            teardown,
            subscriber |> Subscriber.asDisposable,
            active,
          );

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publish2 = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscriber =
          Subscriber.create2(~onNext, ~onComplete, ctx0, ctx1);
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(
            teardown,
            subscriber |> Subscriber.asDisposable,
            active,
          );

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publish3 = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscriber =
          Subscriber.create3(
            ~onNext,
            ~onComplete,
            ctx0,
            ctx1,
            ctx2,
          );
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(
            teardown,
            subscriber |> Subscriber.asDisposable,
            active,
          );

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publish4 = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscriber =
          Subscriber.create4(
            ~onNext,
            ~onComplete,
            ctx0,
            ctx1,
            ctx2,
            ctx3,
          );
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(
            teardown,
            subscriber |> Subscriber.asDisposable,
            active,
          );

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publish5 = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscriber =
          Subscriber.create5(
            ~onNext,
            ~onComplete,
            ctx0,
            ctx1,
            ctx2,
            ctx3,
            ctx4,
          );
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(
            teardown,
            subscriber |> Subscriber.asDisposable,
            active,
          );

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publish6 = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscriber =
          Subscriber.create6(
            ~onNext,
            ~onComplete,
            ctx0,
            ctx1,
            ctx2,
            ctx3,
            ctx4,
            ctx5,
          );
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(
            teardown,
            subscriber |> Subscriber.asDisposable,
            active,
          );

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publish7 = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscriber =
          Subscriber.create7(
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
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(
            teardown,
            subscriber |> Subscriber.asDisposable,
            active,
          );

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let raise = RaiseObservable.raise;

let repeat = {
  let defaultPredicate =
    fun
    | None => true
    | Some(_) => false;
  (~predicate=Functions.alwaysTrue1) => {
    let predicate =
      predicate === Functions.alwaysTrue1 ?
        defaultPredicate :
        (
          fun
          | None => predicate()
          | Some(_) => false
        );
    RepeatObservable.repeat(predicate);
  };
};

let retry = {
  let defaultPredicate =
    fun
    | None => false
    | Some(_) => true;

  (~predicate=Functions.alwaysTrue1) => {
    let predicate =
      predicate === Functions.alwaysTrue1 ?
        defaultPredicate :
        (
          fun
          | None => false
          | Some(exn) => predicate(exn)
        );

    RepeatObservable.repeat(predicate);
  };
};


let share = observable => MulticastObservable.create(Subject.create, observable);

let shareWithReplayBuffer = count => {
  let createSubject = () => Subject.createWithReplayBuffer(count);
  MulticastObservable.create(createSubject);
};

let startWithList = (~scheduler=?, values, observable) =>
  concat([ofList(~scheduler?, values), observable]);

let startWithValue = (~scheduler=?, value, observable) =>
  concat([ofValue(~scheduler?, value), observable]);

let subscribeOn = SubscribeOnObservable.subscribeOn;

let toList = {
  let toListAccumulator = (acc, next) => [next, ...acc];

  observable =>
    observable
    |> Observable.pipe3(
         ScanOperator.operator(toListAccumulator, []),
         LastOperator.operator,
         MapOperator.operator(Lists.rev),
       );
};
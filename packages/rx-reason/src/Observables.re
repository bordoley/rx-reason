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
        let subscriber = Subscriber.createAutoDisposing(~onNext, ~onComplete);
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(teardown, subscriber |> Subscriber.asDisposable, active);

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
        let subscriber = Subscriber.createAutoDisposing1(~onNext, ~onComplete, ctx0);
        observable |> Observable.subscribeWith(subscriber);
        let newConnection =
          Disposable.create2(teardown, subscriber |> Subscriber.asDisposable, active);

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
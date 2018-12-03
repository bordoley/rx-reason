let combineLatest2 = CombineLatest2Observable.create;

let concat = observable =>
  observable |> RxObservable.lift(RxOperators.concat);

let concatMap = (f, observable) =>
  observable |> RxObservable.lift(RxOperators.concatMap(f));

let concatList = ConcatListObservable.create;

let debounce = (~scheduler, dueTime, observable) =>
  observable |> RxObservable.lift(RxOperators.debounce(~scheduler, dueTime));

let defaultIfEmpty = (default, observable) =>
  observable |> RxObservable.lift(RxOperators.defaultIfEmpty(default));

let defer = DeferObservable.create;

let delay = (~scheduler, delay, observable) =>
  observable |> RxObservable.lift(RxOperators.delay(~scheduler, delay));

let dematerialize = observable =>
  observable |> RxObservable.lift(RxOperators.dematerialize);

let distinctUntilChanged = (~equals=?, observable) =>
  observable |> RxObservable.lift(RxOperators.distinctUntilChanged(~equals?));

let empty = EmptyObservable.create;

let every = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.every(predicate));

let exhaust = observable =>
  observable |> RxObservable.lift(RxOperators.exhaust);

let exhaustMap = (f, observable) =>
  observable |> RxObservable.lift(RxOperators.exhaustMap(f));

let find = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.find(predicate));

let first = observable => observable |> RxObservable.lift(RxOperators.first);

let firstOrNone = observable =>
  observable |> RxObservable.lift(RxOperators.firstOrNone);

let ignoreElements = observable =>
  observable |> RxObservable.lift(RxOperators.ignoreElements);

let isEmpty = observable =>
  observable |> RxObservable.lift(RxOperators.isEmpty);

let keep = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.keep(predicate));

let last = observable => observable |> RxObservable.lift(RxOperators.last);

let lastOrNone = observable =>
  observable |> RxObservable.lift(RxOperators.lastOrNone);

let map = (f, observable) =>
  observable |> RxObservable.lift(RxOperators.map(f));

let mapTo = (value, observable) =>
  observable |> RxObservable.lift(RxOperators.mapTo(value));

let materialize = observable =>
  observable |> RxObservable.lift(RxOperators.materialize);

let maybeFirst = observable =>
  observable |> RxObservable.lift(RxOperators.maybeFirst);

let maybeLast = observable =>
  observable |> RxObservable.lift(RxOperators.maybeLast);

let merge = (~maxBufferSize=?, ~maxConcurrency=?, observable) =>
  observable
  |> RxObservable.lift(RxOperators.merge(~maxBufferSize?, ~maxConcurrency?));

let mergeList = MergeListObservable.create;

let mergeMap = (~maxBufferSize=?, ~maxConcurrency=?, f, observable) =>
  observable
  |> RxObservable.lift(
       RxOperators.mergeMap(~maxBufferSize?, ~maxConcurrency?, f),
     );

let none = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.none(predicate));

let observe = (~onNext, ~onComplete, observable) =>
  observable |> RxObservable.lift(RxOperators.observe(~onNext, ~onComplete));

let observe1 = (~onNext, ~onComplete, ctx0, observable) =>
  observable
  |> RxObservable.lift(RxOperators.observe1(~onNext, ~onComplete, ctx0));

let observe2 = (~onNext, ~onComplete, ctx0, ctx1, observable) =>
  observable
  |> RxObservable.lift(
       RxOperators.observe2(~onNext, ~onComplete, ctx0, ctx1),
     );

let observe3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, observable) =>
  observable
  |> RxObservable.lift(
       RxOperators.observe3(~onNext, ~onComplete, ctx0, ctx1, ctx2),
     );

let observe4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, observable) =>
  observable
  |> RxObservable.lift(
       RxOperators.observe4(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3),
     );

let observe5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, observable) =>
  observable
  |> RxObservable.lift(
       RxOperators.observe5(
         ~onNext,
         ~onComplete,
         ctx0,
         ctx1,
         ctx2,
         ctx3,
         ctx4,
       ),
     );

let observeOn = (scheduler, observable) =>
  observable |> RxObservable.lift(RxOperators.observeOn(scheduler));

let ofList = OfListObservable.create;

let ofNotifications = OfNotificationsObservable.create;

let ofValue = OfValueObservable.create;

let onComplete = (onComplete, observable) =>
  observable |> observe(~onNext=RxFunctions.alwaysUnit1, ~onComplete);

let onExn = (onExn, observable) =>
  observable
  |> observe(
       ~onNext=RxFunctions.alwaysUnit1,
       ~onComplete=
         fun
         | None => ()
         | Some(exn) => onExn(exn),
     );

let onNext = (onNext, observable) =>
  observable |> observe(~onNext, ~onComplete=RxFunctions.alwaysUnit1);

let onNext1 = (onNext, ctx0, observable) =>
  observable |> observe1(~onNext, ~onComplete=RxFunctions.alwaysUnit2, ctx0);

let onNext2 = (onNext, ctx0, ctx1, observable) =>
  observable
  |> observe2(~onNext, ~onComplete=RxFunctions.alwaysUnit3, ctx0, ctx1);

let onSubscribe = (f, observable) =>
  observable |> RxObservable.lift(RxOperators.onSubscribe(f));

let raise = RaiseObservable.create;

let repeat = {
  let defaultPredicate =
    fun
    | None => true
    | Some(_) => false;
  (~predicate=RxFunctions.alwaysTrue1) => {
    let predicate =
      predicate === RxFunctions.alwaysTrue1 ?
        defaultPredicate :
        (
          fun
          | None => predicate()
          | Some(_) => false
        );
    RepeatObservable.create(predicate);
  };
};

let retry = {
  let defaultPredicate =
    fun
    | None => false
    | Some(_) => true;

  (~predicate=RxFunctions.alwaysTrue1) => {
    let predicate =
      predicate === RxFunctions.alwaysTrue1 ?
        defaultPredicate :
        (
          fun
          | None => false
          | Some(exn) => predicate(exn)
        );

    RepeatObservable.create(predicate);
  };
};

let scan = (reducer, initialValue, observable) =>
  observable |> RxObservable.lift(RxOperators.scan(reducer, initialValue));

let share = observable =>
  MulticastObservable.create(RxSubject.createMulticast, observable);

let shareWithReplayBuffer = count => {
  let createSubject = () => RxReplayBufferSubject.create(count);
  obs => MulticastObservable.create(createSubject, obs);
};

let some = (predicate, observable) =>
  observable |> RxObservable.lift(RxOperators.some(predicate));

let startWithList = (~scheduler=?, values, observable) =>
  concatList([ofList(~scheduler?, values), observable]);

let startWithValue = (~scheduler=?, value, observable) =>
  concatList([ofValue(~scheduler?, value), observable]);

let subscribeOn = SubscribeOnObservable.create;

let switch_ = observable =>
  observable |> RxObservable.lift(RxOperators.switch_);

let switchMap = (f, observable) =>
  observable |> RxObservable.lift(RxOperators.switchMap(f));

let timeout = (~scheduler, due, observable) =>
  observable |> RxObservable.lift(RxOperators.timeout(~scheduler, due));

let withLatestFrom = (~selector, other, source) =>
  source |> RxObservable.lift(RxOperators.withLatestFrom(~selector, other));
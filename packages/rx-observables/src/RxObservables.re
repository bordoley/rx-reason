let combineLatest2 = CombineLatest2Observable.create;

let concat = observable =>
  observable |> RxObservable.lift(ConcatOperator.create);

let concatMap = (f, observable) =>
  observable |> RxObservable.lift(ConcatMapOperator.create(f));

let concatList = ConcatListObservable.create;

let debounce = (~scheduler, dueTime, observable) =>
  observable
  |> RxObservable.lift(DebounceOperator.create(~scheduler, dueTime));

let defaultIfEmpty = (default, observable) =>
  observable |> RxObservable.lift(DefaultIfEmptyOperator.create(default));

let defer = DeferObservable.create;

let delay = (~scheduler, delay, observable) =>
  observable |> RxObservable.lift(DelayOperator.create(~scheduler, delay));

let dematerialize = observable =>
  observable |> RxObservable.lift(DematerializeOperator.create);

let distinctUntilChanged = (~equals=?, observable) =>
  observable
  |> RxObservable.lift(DistinctUntilChangedOperator.create(~equals?));

let empty = EmptyObservable.create;

let every = (predicate, observable) =>
  observable |> RxObservable.lift(EveryOperator.create(predicate));

let exhaust = observable =>
  observable |> RxObservable.lift(ExhaustOperator.create);

let exhaustMap = (f, observable) =>
  observable |> RxObservable.lift(ExhaustMapOperator.create(f));

let find = (predicate, observable) =>
  observable |> RxObservable.lift(FindOperator.create(predicate));

let first = observable =>
  observable |> RxObservable.lift(FirstOperator.create);

let firstOrNone = observable =>
  observable |> RxObservable.lift(FirstOrNoneOperator.create);

let ignoreElements = observable =>
  observable |> RxObservable.lift(IgnoreElementsOperator.create);

let isEmpty = observable =>
  observable |> RxObservable.lift(IsEmptyOperator.create);

let keep = (predicate, observable) =>
  observable |> RxObservable.lift(KeepOperator.create(predicate));

let last = observable => observable |> RxObservable.lift(LastOperator.create);

let lastOrNone = observable =>
  observable |> RxObservable.lift(LastOrNoneOperator.create);

let map = (f, observable) =>
  observable |> RxObservable.lift(MapOperator.create(f));

let mapTo = (value, observable) =>
  observable |> RxObservable.lift(MapToOperator.create(value));

let materialize = observable =>
  observable |> RxObservable.lift(MaterializeOperator.create);

let maybeFirst = observable =>
  observable |> RxObservable.lift(MaybeFirstOperator.create);

let maybeLast = observable =>
  observable |> RxObservable.lift(MaybeLastOperator.create);

let merge = (~maxBufferSize=?, ~maxConcurrency=?, observable) =>
  observable
  |> RxObservable.lift(
       MergeOperator.create(~maxBufferSize?, ~maxConcurrency?),
     );

let mergeList = MergeListObservable.create;

let mergeMap = (~maxBufferSize=?, ~maxConcurrency=?, f, observable) =>
  observable
  |> RxObservable.lift(
       MergeMapOperator.create(~maxBufferSize?, ~maxConcurrency?, f),
     );

let none = (predicate, observable) =>
  observable |> RxObservable.lift(NoneOperator.create(predicate));

let observe = (~onNext, ~onComplete, observable) =>
  observable
  |> RxObservable.lift(ObserveOperator.create(~onNext, ~onComplete));

let observe1 = (~onNext, ~onComplete, ctx0, observable) =>
  observable
  |> RxObservable.lift(ObserveOperator.create1(~onNext, ~onComplete, ctx0));

let observe2 = (~onNext, ~onComplete, ctx0, ctx1, observable) =>
  observable
  |> RxObservable.lift(
       ObserveOperator.create2(~onNext, ~onComplete, ctx0, ctx1),
     );

let observe3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, observable) =>
  observable
  |> RxObservable.lift(
       ObserveOperator.create3(~onNext, ~onComplete, ctx0, ctx1, ctx2),
     );

let observe4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, observable) =>
  observable
  |> RxObservable.lift(
       ObserveOperator.create4(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3),
     );

let observe5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, observable) =>
  observable
  |> RxObservable.lift(
       ObserveOperator.create5(
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
  observable |> RxObservable.lift(ObserveOnOperator.create(scheduler));

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
  observable |> RxObservable.lift(OnSubscribeOperator.create(f));

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
  observable |> RxObservable.lift(ScanOperator.create(reducer, initialValue));

let share = observable =>
  MulticastObservable.create(RxSubject.createMulticast, observable);

let shareWithReplayBuffer = count => {
  let createSubject = () => RxReplayBufferSubject.create(count);
  obs => MulticastObservable.create(createSubject, obs);
};

let some = (predicate, observable) =>
  observable |> RxObservable.lift(SomeOperator.create(predicate));

let startWithList = (~scheduler=?, values, observable) =>
  concatList([ofList(~scheduler?, values), observable]);

let startWithValue = (~scheduler=?, value, observable) =>
  concatList([ofValue(~scheduler?, value), observable]);

let subscribeOn = SubscribeOnObservable.create;

let switch_ = observable =>
  observable |> RxObservable.lift(SwitchOperator.create);

let switchMap = (f, observable) =>
  observable |> RxObservable.lift(SwitchMapOperator.create(f));

let timeout = (~scheduler, due, observable) =>
  observable |> RxObservable.lift(TimeoutOperator.create(~scheduler, due));

let withLatestFrom = (~selector, other, source) =>
  source |> RxObservable.lift(WithLatestFromOperator.create(~selector, other));
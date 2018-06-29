type t('a) = ObservableSource.t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  include ObservableLike.S1 with type t('a) := t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);
};

let combineLatest2 = CombineLatestObservableSource.combineLatest2;
let combineLatest3 = CombineLatestObservableSource.combineLatest3;
let combineLatest4 = CombineLatestObservableSource.combineLatest4;
let combineLatest5 = CombineLatestObservableSource.combineLatest5;
let combineLatest6 = CombineLatestObservableSource.combineLatest6;
let combineLatest7 = CombineLatestObservableSource.combineLatest7;
let concat = ConcatObservableSource.concat;
let create = ObservableSource.create;
let create1 = ObservableSource.create1;
let create2 = ObservableSource.create2;
let create3 = ObservableSource.create3;
let create4 = ObservableSource.create4;
let create5 = ObservableSource.create5;
let debounce = DebounceOperator.lift;
let defaultIfEmpty = DefaultIfEmptyOperator.lift;
let defer = DeferObservableSource.defer;
let delay = DelayOperator.lift;
let distinctUntilChanged = DistinctUntilChangedOperator.lift;
let empty = EmptyObservableSource.empty;
let every = EveryOperator.lift;
let exhaust = ExhaustOperator.lift;
let find = FindOperator.lift;
let first = FirstOperator.lift;
let firstOrNone = FirstOrNoneOperator.lift;
let ignoreElements = IgnoreElementsOperator.lift;
let isEmpty = IsEmptyOperator.lift;
let keep = KeepOperator.lift;
let last = LastOperator.lift;
let lastOrNone = LastOrNoneOperator.lift;
let lift = ObservableSource.lift;
let map = MapOperator.lift;
let mapTo = MapToOperator.lift;
let materialize = MaterializeOperator.lift;
let maybeFirst = MaybeFirstOperator.lift;
let maybeLast = MaybeLastOperator.lift;
let merge = MergeObservableSource.merge;
let never = ObservableSource.never;
let none = NoneOperator.lift;
let observe = ObserveOperator.lift;
let observeOn = ObserveOnOperator.lift;
let ofAbsoluteTimeNotifications = OfObservableSource.ofAbsoluteTimeNotifications;
let ofList = OfObservableSource.ofList;
let ofNotifications = OfObservableSource.ofNotifications;
let ofRelativeTimeNotifications = OfObservableSource.ofRelativeTimeNotifications;
let ofValue = OfObservableSource.ofValue;
let onComplete = OnCompleteOperator.lift;
let onNext = OnNextOperator.lift;
let onSubscribe = OnSubscribeOperator.lift;
let publish = ObservableSource.publish;
let publish1 = ObservableSource.publish1;
let publishTo = ObservableSource.publishTo;
let publishTo1 = ObservableSource.publishTo1;
let raise = RaiseObservableSource.raise;
let scan = ScanOperator.lift;
let some = SomeOperator.lift;
let switch_ = SwitchOperator.lift;
let subscribeSubscriber = ObservableSource.subscribeSubscriber;
let subscribeWith = ObservableSource.subscribeWith;
let subscribeWith1 = ObservableSource.subscribeWith1;
let subscribeWith2 = ObservableSource.subscribeWith2;
let subscribeWith3 = ObservableSource.subscribeWith3;
let subscribeWith4 = ObservableSource.subscribeWith4;
let subscribeWith5 = ObservableSource.subscribeWith5;
let subscribeWith6 = ObservableSource.subscribeWith6;
let subscribeWith7 = ObservableSource.subscribeWith7;
let subscribe = ObservableSource.subscribe;
let subscribe1 = ObservableSource.subscribe1;
let subscribe2 = ObservableSource.subscribe2;
let subscribe3 = ObservableSource.subscribe3;
let subscribe4 = ObservableSource.subscribe4;
let subscribe5 = ObservableSource.subscribe5;
let subscribe6 = ObservableSource.subscribe6;
let subscribe7 = ObservableSource.subscribe7;
let subscribeOn = SubscribeOnObservableSource.subscribeOn;
let synchronize = SynchronizeOperator.lift;
let timeout = TimeoutOperator.lift;
let withLatestFrom = WithLatestFromOperator.lift;

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
    observable => RepeatOperator.lift(predicate, observable);
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

    observable => RepeatOperator.lift(predicate, observable);
  };
};

let startWithList = (~scheduler=?, values, observable) =>
  concat([ofList(~scheduler?, values), observable]);

let startWithValue = (~scheduler=?, value, observable) =>
  concat([ofValue(~scheduler?, value), observable]);

let toList = {
  let toListAccumulator = (acc, next) => [next, ...acc];

  observable =>
    observable |> scan(toListAccumulator, []) |> last |> map(Lists.rev);
};
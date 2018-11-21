let concat = ConcatObservable.concat;
let defer = DeferObservable.defer;
let empty = EmptyObservable.empty;
let merge = MergeObservable.merge;
let ofAbsoluteTimeNotifications = OfObservable.ofAbsoluteTimeNotifications;
let ofList = OfObservable.ofList;
let ofNotifications = OfObservable.ofNotifications;
let ofRelativeTimeNotifications = OfObservable.ofRelativeTimeNotifications;
let ofValue = OfObservable.ofValue;
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
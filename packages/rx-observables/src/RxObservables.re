let concat = ConcatObservable.concat;
let defer = DeferObservable.defer;
let empty = EmptyObservable.empty;
let merge = MergeObservable.merge;

let ofAbsoluteTimeNotifications = OfObservable.ofAbsoluteTimeNotifications;
let ofList = OfObservable.ofList;
let ofNotifications = OfObservable.ofNotifications;
let ofRelativeTimeNotifications = OfObservable.ofRelativeTimeNotifications;
let ofValue = OfObservable.ofValue;

let pipe2 = (op0, op1, observable) => {
  let op = subscriber => op0 @@ op1 @@ subscriber;
  observable |> RxObservable.lift(op);
};

let pipe3 = (op0, op1, op2, observable) => {
  let op = subscriber => op0 @@ op1 @@ op2 @@ subscriber;
  observable |> RxObservable.lift(op);
};

let pipe4 = (op0, op1, op2, op3, observable) => {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
  observable |> RxObservable.lift(op);
};

let pipe5 = (op0, op1, op2, op3, op4, observable) => {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
  observable |> RxObservable.lift(op);
};

let pipe6 = (op0, op1, op2, op3, op4, op5, observable) => {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
  observable |> RxObservable.lift(op);
};

let pipe7 = (op0, op1, op2, op3, op4, op5, op6, observable) => {
  let op = subscriber =>
    op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
  observable |> RxObservable.lift(op);
};

let publish = ConnectableObservable.publish;
let publish1 = ConnectableObservable.publish1;
let publish2 = ConnectableObservable.publish2;
let publish3 = ConnectableObservable.publish3;
let publish4 = ConnectableObservable.publish4;
let publish5 = ConnectableObservable.publish5;
let publish6 = ConnectableObservable.publish6;
let publish7 = ConnectableObservable.publish7;

let raise = RaiseObservable.raise;

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
    RepeatObservable.repeat(predicate);
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

    RepeatObservable.repeat(predicate);
  };
};

let share = observable =>
  MulticastObservable.create(RxSubject.create, observable);

let shareWithReplayBuffer = count => {
  let createSubject = () => RxSubject.createWithReplayBuffer(count);
  obs => MulticastObservable.create(createSubject, obs);
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
    |> pipe3(
         ScanOperator.operator(toListAccumulator, []),
         LastOperator.operator,
         MapOperator.operator(Lists.rev),
       );
};
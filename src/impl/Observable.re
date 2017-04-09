type t 'a = (Observer.t 'a) => Subscription.t;

let buffer count::(count: int) skip::(skip: int) (observable: t 'a): (t (list 'a)) =>
  failwith "unimplemented";

let combineLatest2With (resultSelector: 'a => 'b => 'c) (a: t 'a) (b: t 'b): (t 'c) =>
 failwith "unimplemented";

let combineLatest3With (resultSelector: 'a => 'b => 'c => 'd) (a: t 'a) (b: t 'b) (c: t 'c): (t 'd) =>
  failwith "unimplemented";

let combineLatest4With (resultSelector: 'a => 'b => 'c => 'd => 'e) (a: t 'a) (b: t 'b) (c: t 'c) (d: t 'd): (t 'e) =>
  failwith "unimplemented";

let combineLatest5With (resultSelector: 'a => 'b => 'c => 'd => 'e => 'f) (a: t 'a) (b: t 'b) (c: t 'c) (d: t 'd) (e: t 'e): (t 'f) =>
  failwith "unimplemented";

let concat (observables: list (t 'a)): (t 'a) =>
  failwith "unimplemented";

let create (f: onNext::('a => unit) => onCompleted::(option exn => unit) => Subscription.t): (t 'a) =>
  failwith "unimplemented";

let defer (f: unit => t 'a): (t 'a) =>
  failwith "unimplemented";

let distinctUntilChangedWith (equals: Immutable.Equality.t 'a) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let doOnCompleted (onCompleted: (option exn) => unit) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let doOnNext (onNext: 'a => unit) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let empty (): (t 'a) =>
  failwith "unimplemented";

let first (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let from (iterable: Immutable.Iterable.t 'a): (t 'a) =>
  failwith "unimplemented";

let filter (f: 'a => bool) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let flatMap (f: 'a => t 'b) (observable: t 'a): (t 'b) =>
  failwith "unimplemented";

let flatten (observables: t (t 'a)): (t 'a) =>
  failwith "unimplemented";

let generate (f: 'a => 'a) (initialValue: 'a): (t 'a) =>
  failwith "unimplemented";

let map (f: 'a => 'b) (observable: t 'a): (t 'b) =>
  failwith "unimplemented";

let merge (observables: list (t 'a)): (t 'a) =>
  failwith "unimplemented";

let observeOn (scheduler: Scheduler.t) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let return (value: 'a): (t 'a) =>
  failwith "unimplemented";

let scan (reducer: 'acc => 'a => 'acc) (initialValue: 'acc) (observable: t 'a): (t 'acc) =>
  failwith "unimplemented";

let skip (count: int) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let skipWhile (f: 'a => bool) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let startWith (value: 'a) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let take (count: int) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let takeWhile (f: 'a => bool) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let subscribe (observer: Observer.t 'a) (observable: t 'a): Subscription.t =>
  observable observer;

let subscribeDefault (observable: t 'a): Subscription.t =>
  observable |> subscribe (Observer.default ());

let subscribeOn (scheduler: Scheduler.t) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let subscribeSafe (observer: Observer.t 'a) (observable: t 'a): Subscription.t =>
  try { observable observer }
  { | exn => observer |> Observer.onCompleted (Some exn); Subscription.empty };

let toObservable (observable: t 'a): (t 'a) => observable;

let withLatestFrom (resultSelector: 'a => 'b => 'c) (b: t 'b) (a: t 'a): (t 'c) =>
    failwith "unimplemented";

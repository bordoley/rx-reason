let module AsyncIterator = {
  type t 'a 'asyncIterable  = {
    reduce: 'acc .
      (while_::('acc => 'a => (Observable.t bool)) =>
      ('acc => 'a => (Observable.t 'acc)) =>
      'acc =>
      'asyncIterable =>
      (Observable.t 'acc)),
  };
};

type t 'a =
  | Empty
  | AsyncIterable 'asyncIterable (AsyncIterator.t 'a 'asyncIterable): t 'a;

let alwaysTrue2Async _ _ => Observable.return true;

let reduce
    while_::(while_: 'acc => 'a => (Observable.t bool))=alwaysTrue2Async
    (reducer: 'acc => 'a => (Observable.t 'acc))
    (initialValue: 'acc)
    (asyncIterable: t 'a): (Observable.t 'acc) =>
  failwith "unimplemented";

let buffer count::(count: int) skip::(skip: int) (asyncIterable: t 'a): (t (list 'a)) =>
  failwith "unimplemented";

let concat (asyncIterables: list (t 'a)): (t 'a) =>
  failwith "unimplemented";

let defer (f: unit => t 'a): (t 'a) =>
  failwith "unimplemented";

let distinctUntilChangedWith (equals: 'a => 'a => bool) (asyncIterable: t 'a): (t 'a) =>
  failwith "unimplemented";

let doOnNext (onNext: 'a => unit) (asyncIterable: t 'a): (t 'a) =>
  failwith "unimplemented";

let filter (f: 'a => bool) (asyncIterable: t 'a): (t 'a) =>
  failwith "unimplemented";

let flatMap (f: 'a => t 'b) (asyncIterable: t 'a): (t 'b) =>
  failwith "unimplemented";

let flatten (asyncIterables: t (t 'a)): (t 'a) =>
  failwith "unimplemented";

let generate (f: 'a => 'a) (initialValue: 'a): (t 'a) =>
  failwith "unimplemented";

let map (f: 'a => 'b) (asyncIterable: t 'a): (t 'b) =>
  failwith "unimplemented";

let return (value: 'a): (t 'a) =>
  failwith "unimplemented";

let scan (reducer: 'acc => 'a => 'acc) (initialValue: 'acc) (asyncIterable: t 'a): (t 'acc) =>
  failwith "unimplemented";

let skip (count: int) (asyncIterable: t 'a): (t 'a) =>
  failwith "unimplemented";

let skipWhile (f: 'a => bool) (asyncIterable: t 'a): (t 'a) =>
  failwith "unimplemented";

let startWith (value: 'a) (asyncIterable: t 'a): (t 'a) =>
  failwith "unimplemented";

let take (count: int) (asyncIterable: t 'a): (t 'a) =>
  failwith "unimplemented";

let takeWhile (f: 'a => bool) (asyncIterable: t 'a): (t 'a) =>
  failwith "unimplemented";

let toAsyncIterable (asyncIterable: t 'a): (t 'a) => asyncIterable;

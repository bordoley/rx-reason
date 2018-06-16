/**
 * A notification provider that is guaranteed to complete with either 
 * a single value or an exception. Conceptually similar to a Promise or Task.
 */;
type t('a);

/** Cast to Observable.t. */
let asObservable: t('a) => Observable.t('a);

let create:
  ((~onSuccess: 'a => unit, ~onError: exn => unit) => Disposable.t) => t('a);

let defer: (unit => t('a)) => t('a);

let every: ('a => bool, Observable.t('a)) => t(bool);

let find: ('a => bool, Observable.t('a)) => t('a);

let first: Observable.t('a) => t('a);

let flatMap: ('a => t('b), t('a)) => t('b);

let last: Observable.t('a) => t('a);

let liftFirst: (Operator.t('a, 'b), Observable.t('a)) => t('b);

let liftLast: (Operator.t('a, 'b), Observable.t('a)) => t('b);

let map: ('a => 'b, t('a)) => t('b);

let none: ('a => bool, Observable.t('a)) => t(bool);

let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

let raise: (~scheduler: Scheduler.t=?, exn) => t('a);

let reduce: (('acc, 'a) => 'acc, 'acc, Observable.t('a)) => t('acc);

let some: ('a => bool, Observable.t('a)) => t(bool);

let subscribe:
  (~onSuccess: ('a => unit)=?, ~onError: (exn => unit)=?, t('a)) => Disposable.t;

let subscribeWith:
  (~onSuccess: 'a => unit, ~onError: exn => unit, t('a)) => Disposable.t;
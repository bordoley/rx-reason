type t('a);

let create:
  (
    (~onSuccess: 'a => unit, ~onError: exn => unit) =>
    Disposable.t
  ) =>
  t('a);

let defer: (unit => t('a)) => t('a);

let every: ('a => bool, Observable.t('a)) => t(bool);

let find: ('a => bool, Observable.t('a)) => t('a);

let first: Observable.t('a) => t('a);

let last: Observable.t('a) => t('a);

let liftFirst: (Operator.t('a, 'b), Observable.t('a)) => t('b);

let liftLast: (Operator.t('a, 'b), Observable.t('a)) => t('b);

let none: ('a => bool, Observable.t('a)) => t(bool);

let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

let reduce: (('acc, 'a) => 'acc, 'acc, Observable.t('a)) => t('acc);

let some: ('a => bool, Observable.t('a)) => t(bool);

let subscribe: t('a) => Disposable.t;

let subscribeWithCallbacks:
  (~onSuccess: 'a => unit, ~onError: exn => unit, t('a)) =>
  Disposable.t;

let toObservable: t('a) => Observable.t('a);

type t('a);

let create:
  (
    (~onNext: 'a => unit, ~onComplete: (option(exn)) => unit) =>
    Disposable.t
  ) =>
  t('a);

let defer: (unit => t('a)) => t('a);

let empty: (~scheduler: Scheduler.t=?, unit) => t('a);

let every: ('a => bool, Observable.t('a)) => t(bool);

let find: ('a => bool, Observable.t('a)) => t('a);

let first: Observable.t('a) => t('a);

let last: Observable.t('a) => t('a);

let liftFirst: (Operator.t('a, 'b), Observable.t('a)) => t('b);

let liftLast: (Operator.t('a, 'b), Observable.t('a)) => t('b);

let none: ('a => bool, Observable.t('a)) => t(bool);

let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

let raise: (~scheduler: Scheduler.t=?, exn) => t('a);

let reduce: (('acc, 'a) => 'acc, 'acc, Observable.t('a)) => t('acc);

let some: ('a => bool, Observable.t('a)) => t(bool);

let subscribe: t('a) => Disposable.t;

let subscribeWithCallbacks:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
  Disposable.t;

let subscribeObserver: (Observer.t('a), t('a)) => Disposable.t;

let toObservable: t('a) => Observable.t('a);

let toSingle: t('a) => Single.t(option('a));

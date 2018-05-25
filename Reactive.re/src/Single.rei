type t('a);

let create:
  (
    (~onNext: 'a => unit, ~onComplete: (option(exn)) => unit) =>
    Disposable.t
  ) =>
  t('a);

let defer: (unit => t('a)) => t('a);

let first: Observable.t('a) => t('a);

let last: Observable.t('a) => t('a);

let liftFirst: (Operator.t('a, 'b), t('a)) => t('b);

let liftLast: (Operator.t('a, 'b), t('a)) => t('b);

let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

let reduce: (('acc, 'a) => 'acc, 'acc, Observable.t('a)) => t('acc);

let subscribe:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
  Disposable.t;

let subscribeObserver: (Observer.t('a), t('a)) => Disposable.t;

let toObservable: t('a) => Observable.t('a);
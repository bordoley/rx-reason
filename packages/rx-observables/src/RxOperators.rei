let debounce:
  (~scheduler: RxScheduler.t, ~dueTime: float) => RxOperator.t('a, 'a);

let defaultIfEmpty: 'a => RxOperator.t('a, 'a);

let dematerialize: RxOperator.t(RxNotification.t('a), 'a);

let delay: (~scheduler: RxScheduler.t, ~delay: float) => RxOperator.t('a, 'a);

let distinctUntilChanged:
  (~equals: ('a, 'a) => bool=?) => RxOperator.t('a, 'a);

let every: ('a => bool) => RxOperator.t('a, bool);

let exhaust: RxOperator.t(RxObservable.t('a), 'a);

let find: ('a => bool) => RxOperator.t('a, 'a);

let first: RxOperator.t('a, 'a);

let firstOrNone: RxOperator.t('a, option('a));

let ignoreElements: RxOperator.t('a, 'a);

let isEmpty: RxOperator.t('a, bool);

let keep: ('a => bool) => RxOperator.t('a, 'a);

let last: RxOperator.t('a, 'a);

let lastOrNone: RxOperator.t('a, option('a));

let map: ('a => 'b) => RxOperator.t('a, 'b);

let mapTo: 'b => RxOperator.t('a, 'b);

let materialize: RxOperator.t('a, RxNotification.t('a));

let maybeFirst: RxOperator.t('a, 'a);

let maybeLast: RxOperator.t('a, 'a);

let none: ('a => bool) => RxOperator.t('a, bool);

let observe:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) =>
  RxOperator.t('a, 'a);

let observeOn: RxScheduler.t => RxOperator.t('a, 'a);

let onComplete: (option(exn) => unit) => RxOperator.t('a, 'a);

let onExn: (exn => unit) => RxOperator.t('a, 'a);

let onNext: ('a => unit) => RxOperator.t('a, 'a);

let onSubscribe: (unit => RxDisposable.t) => RxOperator.t('a, 'a);

let scan: (('acc, 'a) => 'acc, 'acc) => RxOperator.t('a, 'acc);

let some: ('a => bool) => RxOperator.t('a, bool);

let switch_: RxOperator.t(RxObservable.t('a), 'a);

let synchronize: RxOperator.t('a, 'a);

let timeout: (~scheduler: RxScheduler.t, ~due: float) => RxOperator.t('a, 'a);

let withLatestFrom:
  (~selector: ('a, 'b) => 'c, RxObservable.t('b)) => RxOperator.t('a, 'c);
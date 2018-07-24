let debounce: (~scheduler: Scheduler.t, ~dueTime: float) => Operator.t('a, 'a);

let defaultIfEmpty: 'a => Operator.t('a, 'a);

let delay:
  (
    ~bufferStrategy: BufferStrategy.t=?,
    ~bufferSize: int=?,
    ~scheduler: Scheduler.t,
    ~delay: float
  ) =>
  Operator.t('a, 'a);

let dematerialize: Operator.t(Notification.t('a), 'a);

let distinctUntilChanged: (~equals: ('a, 'a) => bool=?) => Operator.t('a, 'a);

let every: ('a => bool) => Operator.t('a, bool);

let find: ('a => bool) => Operator.t('a, 'a);

let first: Operator.t('a, 'a);

let firstOrNone: Operator.t('a, option('a));

let ignoreElements: Operator.t('a, 'a);

let isEmpty: Operator.t('a, bool);

let keep: ('a => bool) => Operator.t('a, 'a);

let last: Operator.t('a, 'a);

let lastOrNone: Operator.t('a, option('a));

let map: ('a => 'b) => Operator.t('a, 'b);

let mapTo: 'b => Operator.t('a, 'b);

let materialize: Operator.t('a, Notification.t('a));

let maybeFirst: Operator.t('a, 'a);

let maybeLast: Operator.t('a, 'a);

let none: ('a => bool) => Operator.t('a, bool);

let observe:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) =>
  Operator.t('a, 'a);

let observeOn:
  (~bufferStrategy: BufferStrategy.t=?, ~bufferSize: int=?, Scheduler.t) =>
  Operator.t('a, 'a);

let onComplete: (option(exn) => unit) => Operator.t('a, 'a);

let onNext: ('a => unit) => Operator.t('a, 'a);

let onSubscribe: (unit => Disposable.t) => Operator.t('a, 'a);

let scan: (('acc, 'a) => 'acc, 'acc) => Operator.t('a, 'acc);

let some: ('a => bool) => Operator.t('a, bool);

let synchronize: Operator.t('a, 'a);

let timeout: (~due: float, ~scheduler: Scheduler.t) => Operator.t('a, 'a);
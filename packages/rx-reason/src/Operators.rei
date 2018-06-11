let bufferCount:
  (~size: int, ~startEvery: int=?) => Operator.t('a, array('a));

/*
 let bufferTime:
   (
     ~bufferCreationInterval: float=?,
     ~maxBufferSize: int,
     ~scheduler: DelayScheduler.t=?,
     ~timespan: float,
   ) =>
   /* FIXME: Is list really the right return type... */
   Operator.t('a, list('a));*/

let debounce: (Scheduler.t) => Operator.t('a, 'a);

let defaultIfEmpty: 'a => Operator.t('a, 'a);

let distinctUntilChanged:
  (~comparer: ('a, 'a) => bool=?) => Operator.t('a, 'a);

let every: ('a => bool) => Operator.t('a, bool);

let exhaust: Operator.t(Observable.t('a), 'a);

let find: ('a => bool) => Operator.t('a, 'a);

let first: Operator.t('a, 'a);

let firstOrNone: Operator.t('a, option('a));

let identity: Operator.t('a, 'a);

let ignoreElements: Operator.t('a, 'a);

let isEmpty: Operator.t('a, bool);

let keep: ('a => bool) => Operator.t('a, 'a);

let last: Operator.t('a, 'a);

let lastOrNone: Operator.t('a, option('a));

let map: ('a => 'b) => Operator.t('a, 'b);

let mapTo: 'b => Operator.t('a, 'b);

let maybe: Operator.t('a, 'a);

let maybeFirst: Operator.t('a, 'a);

let maybeLast: Operator.t('a, 'a);

let none: ('a => bool) => Operator.t('a, bool);

let observe:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) =>
  Operator.t('a, 'a);

let observeOn:
  (
    ~backPressureStrategy: BackPressureStrategy.t=?,
    ~bufferSize: int=?,
    Scheduler.t
  ) =>
  Operator.t('a, 'a);

let onComplete: (option(exn) => unit) => Operator.t('a, 'a);

let onNext: ('a => unit) => Operator.t('a, 'a);

let scan: (('acc, 'a) => 'acc, 'acc) => Operator.t('a, 'acc);

let some: ('a => bool) => Operator.t('a, bool);

let switch_: Operator.t(Observable.t('a), 'a);

let synchronize: Operator.t('a, 'a);

let timeout: Scheduler.t => Operator.t('a, 'a);

let withLatestFrom:
  (~selector: ('a, 'b) => 'c, Observable.t('b)) => Operator.t('a, 'c);
type t('a);

let combineLatest2:
  (~selector: ('a, 'b) => 'c, t('a), t('b)) =>
  t('c);

let combineLatest3:
  (
    ~selector: ('a, 'b, 'c) => 'd,
    t('a),
    t('b),
    t('c)
  ) =>
  t('d);

let combineLatest4:
  (
    ~selector: ('a, 'b, 'c, 'd) => 'e,
    t('a),
    t('b),
    t('c),
    t('d)
  ) =>
  t('e);

let combineLatest5:
  (
    ~selector: ('a, 'b, 'c, 'd, 'e) => 'f,
    t('a),
    t('b),
    t('c),
    t('d),
    t('e)
  ) =>
  t('f);

let combineLatest6:
  (
    ~selector: ('a, 'b, 'c, 'd, 'e, 'f) => 'g,
    t('a),
    t('b),
    t('c),
    t('d),
    t('e),
    t('f)
  ) =>
  t('g);

let combineLatest7:
  (
    ~selector: ('a, 'b, 'c, 'd, 'e, 'f, 'g) => 'h,
    t('a),
    t('b),
    t('c),
    t('d),
    t('e),
    t('f),
    t('g)
  ) =>
  t('h);

let concat: (~scheduler: Scheduler.t=?, list(t('a))) => t('a);

let create:
  ((~onNext: 'a => unit, ~onComplete: option(exn) => unit) => Disposable.t) =>
  t('a);

let createWithObserver: (Observer.t('a) => Disposable.t) => t('a);

let defer: (unit => t('a)) => t('a);

let empty: (~scheduler: Scheduler.t=?, unit) => t('a);

let lift: (Operator.t('a, 'b), t('a)) => t('b);

let merge: (list(t('a))) => t('a);

let never: unit => t('a);

let ofList: (~scheduler: Scheduler.t=?, list('a)) => t('a);

let ofValue: (~scheduler: Scheduler.t=?, 'a) => t('a);

let retry: (exn => bool, t('a)) => t('a);

let startWithList: (~scheduler: Scheduler.t=?, list('a), t('a)) => t('a);

let startWithValue: (~scheduler: Scheduler.t=?, 'a, t('a)) => t('a);

let subscribe: t('a) => Disposable.t;

let subscribeObserver: (Observer.t('a), t('a)) => Disposable.t;

let subscribeWithCallbacks:
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
  Disposable.t;

let subscribeOn: (Scheduler.t, t('a)) => t('a);

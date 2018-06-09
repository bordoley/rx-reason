type t('a);

include Disposable.S1 with type t('a) := t('a);

let complete: (option(exn), t('a)) => unit;

let completeWithResult: (option(exn), t('a)) => bool;

let create:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    ~onDispose: unit => unit
  ) =>
  t('a);

let createAutoDisposing:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    ~onDispose: unit => unit
  ) =>
  t('a);

let disposed: t('a);

let isStopped: t('a) => bool;

let next: ('a, t('a)) => unit;
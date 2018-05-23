type t('a);

let complete: (~exn: option(exn)=?, t('a)) => unit;

let completeWithResult: (~exn: option(exn)=?, t('a)) => bool;

let create:
  (
    ~onComplete: option(exn) => unit=?,
    ~onNext: 'a => unit=?,
    ~onDispose: unit => unit=?,
    unit
  ) =>
  t('a);

let next: ('a, t('a)) => unit;

let toDisposable: t('a) => Disposable.t;
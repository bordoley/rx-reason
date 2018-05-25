type t('a);

let complete: (option(exn), t('a)) => unit;

let completeWithResult: (option(exn), t('a)) => bool;

let create:
  (
    ~onNext: 'a => unit,
    ~onComplete: option(exn) => unit,
    ~onDispose: unit => unit,
  ) =>
  t('a);

let next: ('a, t('a)) => unit;

let toDisposable: t('a) => Disposable.t;
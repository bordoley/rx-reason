/**
 * Represents reactive event types and their values.
 */;

type t('a);

let complete: option(exn) => t('a);

let next: 'a => t('a);

let map: (~onNext: 'a => 'b, ~onComplete: option(exn) => 'b, t('a)) => 'b;

let map1:
  (
    ~onNext: ('ctx0, 'a) => 'b,
    ~onComplete: ('ctx0, option(exn)) => 'b,
    'ctx0,
    t('a)
  ) =>
  'b;

let map2:
  (
    ~onNext: ('ctx0, 'ctx1, 'a) => 'b,
    ~onComplete: ('ctx0, 'ctx1, option(exn)) => 'b,
    'ctx0,
    'ctx1,
    t('a)
  ) =>
  'b;
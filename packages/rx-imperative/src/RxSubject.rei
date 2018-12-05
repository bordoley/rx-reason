/**
 * A type that both an Observable sequence as well as an Subscriber.
 */;
type t('a);

include RxDisposable.S1 with type t('a) := t('a);
include RxSubjectLike.S1 with type t('a) := t('a);

let createMulticast: unit => t('a);

let create2:
  (
    ~onNext: ('ctx0, 'ctx1, 'a) => unit,
    ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit,
    ~onSubscribe: ('ctx0, 'ctx1, RxSubscriber.t('a)) => unit,
    ~onDispose: ('ctx0, 'ctx1) => unit,
    'ctx0,
    'ctx1
  ) =>
  t('a);

/**
 * A disposed Subject instance.
 */
let disposed: t('a);
type t('request, 'a);

include RxDisposableLike.S2 with type t('request, 'a) := t('request, 'a);

let asObservable: t('request, 'a) => RxObservable.t('a);

let create: ('request => RxObservable.t('a)) => t('request, 'a);

let create1:
  (('ctx0, 'request) => RxObservable.t('a), 'ctx0) => t('request, 'a);

let disposed: t('request, 'a);

let request: ('request, t('request, 'a)) => unit;
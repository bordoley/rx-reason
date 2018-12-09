type t('a, 'request);

include RxDisposable.S2 with type t('a, 'request) := t('a, 'request);

let asObservable: t('a, 'request) => RxObservable.t('a);

let create: (
  ~request: ('request, 'resource) => RxObservable.t('a),
  ~disposeResource: 'resource => unit,
  'resource,
) => t('a, 'request);

let disposed: t('a, 'request);

let request: ('request, t('a, 'request)) => unit;
let create: (
  ~createResource: unit => 'resource,
  ~disposeResource: 'resource => unit,
  ~request: ('request, 'resource) => RxObservable.t('a),
  unit,
) => RxAsyncIterator.t('a, 'request);

let create1: (
  ~createResource: 'ctx0 => 'resource,
  ~disposeResource: 'resource => unit,
  ~request: ('request, 'resource) => RxObservable.t('a),
  'ctx0,
) => RxAsyncIterator.t('a, 'request);

let create2: (
  ~createResource: ('ctx0, 'ctx1) => 'resource,
  ~disposeResource: 'resource => unit,
  ~request: ('request, 'resource) => RxObservable.t('a),
  'ctx0,
  'ctx1,
) => RxAsyncIterator.t('a, 'request);

let create3: (
  ~createResource: ('ctx0, 'ctx1, 'ctx2) => 'resource,
  ~disposeResource: 'resource => unit,
  ~request: ('request, 'resource) => RxObservable.t('a),
  'ctx0,
  'ctx1,
  'ctx2,
) => RxAsyncIterator.t('a, 'request);
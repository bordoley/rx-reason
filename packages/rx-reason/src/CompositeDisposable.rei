type t;

type compositeDisposable = t;

module type S = {
  type t;

  include Disposable.S with type t := t;

  let addDisposable: (Disposable.t, t) => t;

  let addTeardown: (TeardownLogic.t, t) => t;

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t => compositeDisposable;
};

module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let addDisposable: (Disposable.t, t('a)) => t('a);

  let addTeardown: (TeardownLogic.t, t('a)) => t('a);

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t('a) => compositeDisposable;
};

include S with type t := t;

/** Constructs a new CompositeDisposable instance. */
let create: unit => t;

let disposed: t;
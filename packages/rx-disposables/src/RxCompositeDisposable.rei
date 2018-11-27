/**
 *  A disposable container that references other disposables and teardown logic.
 */;
type t;

type compositeDisposable = t;

module type S = {
  type t;

  include RxCompositeDisposableLike.S with type t := t;
  include RxDisposable.S with type t := t;

  /** Cast to CompositeDisposable.t. */
  let asCompositeDisposable: t => compositeDisposable;
};

module type S1 = {
  type t('a);

  include RxCompositeDisposableLike.S1 with type t('a) := t('a);
  include RxDisposable.S1 with type t('a) := t('a);

  /** Cast to CompositeDisposable.t. */
  let asCompositeDisposable: t('a) => compositeDisposable;
};

include RxDisposable.S with type t := t;
include RxCompositeDisposableLike.S with type t := t;

/** Constructs a new CompositeDisposable instance. */
let create: unit => t;

/**
 * A disposed CompositeDisposable instance.
 */
let disposed: t;
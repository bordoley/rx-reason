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

module type S2 = {
  type t('a, 'b);

  include RxCompositeDisposableLike.S2 with type t('a, 'b) := t('a, 'b);
  include RxDisposable.S2 with type t('a, 'b) := t('a, 'b);

  /** Cast to CompositeDisposable.t. */
  let asCompositeDisposable: t('a, 'b) => compositeDisposable;
};

include RxDisposable.S with type t := t;
include RxCompositeDisposableLike.S with type t := t;

/** Constructs a new CompositeDisposable instance. */
let create: unit => t;

/**
 * A disposed CompositeDisposable instance.
 */
let disposed: t;
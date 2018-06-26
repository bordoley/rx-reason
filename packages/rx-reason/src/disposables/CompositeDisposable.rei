/**
 *  A disposable container that references other disposables and teardown logic.
 */;
type t;

type compositeDisposable = t;

module type S = {
  type t;

  include CompositeDisposableLike.S with type t := t;
  include Disposable.S with type t := t;

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t => compositeDisposable;
};

module type S1 = {
  type t('a);

  include CompositeDisposableLike.S1 with type t('a) := t('a);
  include Disposable.S1 with type t('a) := t('a);

  /** Cast to Disposable.t. */
  let asCompositeDisposable: t('a) => compositeDisposable;
};

include Disposable.S with type t := t;
include CompositeDisposableLike.S with type t := t;

/** Constructs a new CompositeDisposable instance. */
let create: unit => t;

let disposed: t;
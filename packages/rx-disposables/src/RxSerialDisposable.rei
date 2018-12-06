/**
 * A Disposable container that allows atomically updating/replacing the contained
 * Disposable with another Disposable, disposing the old one when updating plus
 * handling the disposition when the container itself is disposed.
 */;

type t;

type serialDisposable = t;

module type S = {
  type t;

  include RxSerialDisposableLike.S with type t := t;
  include RxDisposable.S with type t := t;

  /** Cast to SerialDisposable.t. */
  let asSerialDisposable: t => serialDisposable;
};

module type S1 = {
  type t('a);

  include RxSerialDisposableLike.S1 with type t('a) := t('a);
  include RxDisposable.S1 with type t('a) := t('a);

  /** Cast to SerialDisposable.t. */
  let asSerialDisposable: t('a) => serialDisposable;
};

module type S2 = {
  type t('a, 'b);

  include RxSerialDisposableLike.S2 with type t('a, 'b) := t('a, 'b);
  include RxDisposable.S2 with type t('a, 'b) := t('a, 'b);

  /** Cast to SerialDisposable.t. */
  let asSerialDisposable: t('a, 'b) => serialDisposable;
};

include RxDisposable.S with type t := t;
include RxSerialDisposableLike.S with type t := t;

/** Constructs a new SerialDisposable instance containing a disposed disposable. */
let create: unit => t;

/**
 * A disposed SerialDisposable instance.
 */
let disposed: t;
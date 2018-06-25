/**
 * A Disposable container that allows atomically updating/replacing the contained
 * Disposable with another Disposable, disposing the old one when updating plus
 * handling the disposition when the container itself is disposed.
 */;

type t;

type serialDisposable = t;

/* FIXME: Use more descriptive names than get/set now that these are module types. */
module type S = {
  type t;

  include Disposable.S with type t := t;

  let asSerialDisposable: t => serialDisposable;

  /** Returns the currently contained Disposable */
  let get: t => Disposable.t;

  /** Atomically set the next disposable on this container and dispose the previous
   *  one (if any) or dispose next if the container has been disposed
   */
  let set: (Disposable.t, t) => unit;
};


module type S1 = {
  type t('a);

  include Disposable.S1 with type t('a) := t('a);

  let asSerialDisposable: t('a) => serialDisposable;
  
  /** Returns the currently contained Disposable */
  let get: t('a) => Disposable.t;

  /** Atomically set the next disposable on this container and dispose the previous
   *  one (if any) or dispose next if the container has been disposed
   */
  let set: (Disposable.t, t('a)) => unit;
};

include S with type t := t;

/** Constructs a new SerialDisposable instance containing a disposed disposable. */
let create: unit => t;

let disposed: t;


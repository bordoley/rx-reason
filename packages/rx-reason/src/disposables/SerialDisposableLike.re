module type S = {
  type t;

  include DisposableLike.S with type t := t;

  /** Returns the currently contained Disposable */
  let getInnerDisposable: t => Disposable.t;

  /** Atomically set the next disposable on this container and dispose the previous
   *  one (if any) or dispose next if the container has been disposed
   */
  let setInnerDisposable: (Disposable.t, t) => unit;
};

module type S1 = {
  type t('a);

  include DisposableLike.S1 with type t('a) := t('a);

  /** Returns the currently contained Disposable */
  let getInnerDisposable: t('a) => Disposable.t;

  /** Atomically set the next disposable on this container and dispose the previous
   *  one (if any) or dispose next if the container has been disposed
   */
  let setInnerDisposable: (Disposable.t, t('a)) => unit;
};